#include <memory.h>
#include <sstring.h>
#include <json.h>
#include <streaming.h>
#include <filesystem.h>

/*-------------------------------- NT declarations --------------------------------*/
#pragma region NTDECL
#define MAX_PATH          260
#define FILE_ATTRIBUTE_DIRECTORY	0x00000010

extern "C"
{

typedef struct _FILETIME
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef struct _WIN32_FIND_DATAA
{
	DWORD    dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD    nFileSizeHigh;
	DWORD    nFileSizeLow;
	DWORD    dwReserved0;
	DWORD    dwReserved1;
	char     cFileName[MAX_PATH];
	char     cAlternateFileName[14];
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;
typedef struct _STARTUPINFOA
{
	DWORD   cb;
	char   *lpReserved;
	char   *lpDesktop;
	char   *lpTitle;
	DWORD   dwX;
	DWORD   dwY;
	DWORD   dwXSize;
	DWORD   dwYSize;
	DWORD   dwXCountChars;
	DWORD   dwYCountChars;
	DWORD   dwFillAttribute;
	DWORD   dwFlags;
	WORD    wShowWindow;
	WORD    cbReserved2;
	BYTE   *lpReserved2;
	void   *hStdInput;
	void   *hStdOutput;
	void   *hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;
typedef struct _PROCESS_INFORMATION
{
	void  *hProcess;
	void  *hThread;
	DWORD  dwProcessId;
	DWORD  dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;

void *__stdcall FindFirstFileA(const char *, LPWIN32_FIND_DATAA);
bool __stdcall FindNextFileA(void *, LPWIN32_FIND_DATAA);
bool __stdcall FindClose(void *);
BOOL __stdcall CreateProcessA(const char *, void *, void *, void *, BOOL, DWORD, void *, void *, void *, void *);
DWORD __stdcall GetLastError(void);
DWORD __stdcall WaitForSingleObject(void *, DWORD);
BOOL __stdcall CloseHandle(void *);
BOOL __stdcall GetExitCodeProcess(void *, DWORD *);

}
#pragma endregion NTDECL
/*-------------------------------- NT declarations --------------------------------*/

// project object
class project
{
	public:
	String::string path;       // project's path
	JSON::object config;       // project's config, JSON object
	bool compiled = false;     // whether project has successfully built in current build task

	project(String::string);
	bool compile();
	bool depend();
	String::string include();
	String::string resource();
	String::string source();
	String::string option(int);
	String::string definitions();
	String::string object();
	String::string executable();
};

struct ProjectNode
{
	project *project = nullptr;
	ProjectNode *next = nullptr;
};

// A linked-list, saved all project in current build task
ProjectNode *ProjectList = nullptr;

// Read json string in file
String::string ReadJson(const String::string &filePath)
{
	if (!Filesystem::file(filePath))
	{
		Streaming::cout << "File not found: " << filePath << Streaming::LF;
		throw Memory::exception(2, Memory::DOSERROR);
	}
	Streaming::file file(Filesystem::open(filePath, Filesystem::OF_READ));
	Memory::string buf(file.available());
	/*
	 * Streaming::fully is a blocked-io object,
	 * it will read data from given io object
	 * until enough bytes have been read or some
	 * error occurred
	 */
	Streaming::fully fullyReader(&file);
	fullyReader.read(buf);
	file.close();

	// Memory::string will automatically convert to String::string
	return buf;
}
bool SourceFilter(WIN32_FIND_DATAA *data)
{
	if (data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return false;
	const char *name = data->cFileName;
	QWORD length = String::length(name);
	const char *ext = name + length;
	while (ext >= name && *ext != '.')
	{
		ext--;
	}
	ext++;
	if (ext == name)
		return false;
	return *ext == 'C' || *ext == 'c';
}
bool ResourceFilter(WIN32_FIND_DATAA *data)
{
	char ext[] = ".rc";
	if (data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return false;
	QWORD length = String::length(data->cFileName);
	if (length >= 3)
	{
		length -= 3;
		return Memory::compare(ext, data->cFileName + length, 3);
	}
	return false;
}
// Foreach files in folder and concat them into a string
JSON::object ForeachFiles(const String::string &folder, bool (*filter)(WIN32_FIND_DATAA *))
{
	JSON::object files;
	QWORD idx = 0;
	Memory::string cstr = (folder + "\\*").native();
	WIN32_FIND_DATAA data = {};
	void *handle = FindFirstFileA((const char *) cstr.address, &data);
	if (handle != ((void *) -1))
	{
		bool search = true;
		while (search)
		{
			if (filter(&data))
			{
				files[idx++] = (folder + "\\") + (const char *) data.cFileName;
			}
			data = {};
			search = FindNextFileA(handle, &data);
		}
		FindClose(handle);
	}
	return files;
}
// Find project by name in ProjectList
project *FindProject(const String::string &name)
{
	ProjectNode *node = ProjectList;
	while (node)
	{
		String::string projname = node->project->config["name"];
		if (projname == name)
		{
			return node->project;
		}
		node = node->next;
	}
	return nullptr;
}
// CreateProcess use given cmd string
bool ExecuteCommand(const String::string &cmd, const String::string &work)
{
	Memory::string cstrCMD = cmd.native();
	Memory::string cstrWork = work.native();
	STARTUPINFOA startup = {};
	startup.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION procinfo = {};
	if (!CreateProcessA(nullptr, cstrCMD.address, nullptr, nullptr, true, 0, nullptr, cstrWork.address, &startup, &procinfo))
	{
		Streaming::cout << Memory::message(GetLastError(), Memory::DOSERROR) << Streaming::LF;
		return false;
	}
	WaitForSingleObject(procinfo.hProcess, 0xFFFFFFFFUL);

	DWORD exit = 0;
	if (!GetExitCodeProcess(procinfo.hProcess, &exit))
	{
		Streaming::cout << Memory::message(GetLastError(), Memory::DOSERROR) << Streaming::LF;
		return false;
	}
	CloseHandle(procinfo.hProcess);
	CloseHandle(procinfo.hThread);
	return exit == 0;
}
void FreeList()
{
	ProjectNode *node = ProjectList;
	while (node)
	{
		node->project->~project();
		Memory::free(node->project);
		ProjectNode *next = node->next;
		Memory::free(node);
		node = next;
	}
	node = nullptr;
}
String::string ConcatStringArray(const JSON::object &obj)
{
	JSON::object &arr = (JSON::object &) obj;
	String::string value;
	if (arr.type == JSON::type::ARRAY)
	{
		for (QWORD i = 0; i < arr.length(); i++)
		{
			if (arr[i].type == JSON::type::STRING)
			{
				value += arr[i];
				value += " ";
			}
		}
	}
	return value;
}
String::string EscapeString(const String::string &str)
{
	QWORD len = str.length();
	for (QWORD i = 0; i < str.length(); len += str[i++] == '\\');
	String::string retval;
	retval.address.resize(len);
	QWORD idx = 0;
	for (QWORD i = 0; i < str.length(); i++)
	{
		retval[idx++] = str[i];
		if (str[i] == '\\')
		{
			retval [idx++] = '\\';
		}
	}
	return retval;
}


project::project(String::string path):
path((String::string &&) path),
config(JSON::resolve(ReadJson(this->path + "\\config.json")))
{
	this->config.readonly = true;
	Streaming::cout << "\x1B[104;97m LOADING \x1B[0m ";
	Streaming::cout << ((String::string) this->config["name"]) << " [" << this->path << "]\n";
	// Link this project into project list
	// use header link
	ProjectNode *node = (ProjectNode *) Memory::allocate(sizeof(ProjectNode));
	node->project = this;
	node->next = ProjectList;
	ProjectList = node;

	// load subprojects if it has "subproject" config field
	String::string subprojKey = "subproject";
	if (this->config.contain(subprojKey))
	{
		JSON::object subprojects = this->config[subprojKey];

		for (QWORD i = 0; i < subprojects.length(); i++)
		{
			JSON::object subproj = subprojects[i];

			String::string subprojPath = this->path + "\\" + subproj;
			if (Filesystem::absolute(subproj))
			{
				subprojPath = subproj;
			}
			project *proj = (project *) Memory::allocate(sizeof(project));
			new (proj) project(subprojPath);
		}
	}
}
bool project::compile()
{
	// This project never compiled in current build task
	if (this->compiled)
		return true;

	bool success = this->depend();
	if (!success)
		return false;


	String::string sourceFiles = this->source();

	String::string resourceFiles = this->resource();
	// if project has source file(s), compile and link them
	if (sourceFiles)
	{
		Streaming::cout << "\x1B[46;97m COMPILING \x1B[0m ";
		Streaming::cout << ((String::string) this->config["name"]) << " [" << this->path << "]\n";

		// default compiler is cl.exe
		String::string cmd = "cl.exe ";
		cmd += sourceFiles;
		cmd += this->include();
		cmd += this->definitions();
		cmd += this->object();
		cmd += this->executable();
		cmd += this->option(0);
		cmd += resourceFiles;
		success &= ExecuteCommand(cmd, this->path);
		Streaming::cout << Streaming::LF;
		if (!success)
			return false;
	}
	this->compiled = true;
	return true;
}
bool project::depend()
{
	bool success = true;
	// compile depends
	String::string dependsKey = "depends";
	if (this->config.contain(dependsKey))
	{
		JSON::object &depends = this->config[dependsKey];

		for (QWORD i = 0; success && i < depends.length(); i++)
		{
			JSON::object &depele = depends[i];

			const String::string &depName = depele;
			project *depproj = FindProject(depName);
			if (!depproj)
			{
				Streaming::cout << "Cannot find project [" << depName << "]\n";
				return false;
			}

			success &= depproj->compile();
		}
	}
	return success;
}
String::string project::include()
{
	String::string retval;
	// depends
	String::string dependsKey = "depends";
	if (this->config.contain(dependsKey))
	{
		JSON::object &depends = this->config[dependsKey];
		for (QWORD i = 0; i < depends.length(); i++)
		{
			JSON::object &depele = depends[i];
			const String::string &projname = depele;
			project *proj = FindProject(projname);
			retval += proj->include();
		}
	}

	String::string includeKey = "include";
	if (this->config.contain(includeKey))
	{
		JSON::object &include = this->config[includeKey];
		if (include.type == JSON::type::STRING)
		{
			String::string dir = include;
			if (!Filesystem::absolute(dir))
			{
				dir = Filesystem::canonicalize(this->path + "\\" + dir);
			}
			retval += "/I \"";
			retval += EscapeString(dir);
			retval += "\" ";
		}
		else if (include.type == JSON::type::ARRAY)
		{
			for (QWORD i = 0; i < include.length(); i++)
			{
				String::string dir = include[i];
				if (!Filesystem::absolute(dir))
				{
					dir = Filesystem::canonicalize(this->path + "\\" + dir);
				}
				retval += "/I \"";
				retval += EscapeString(dir);
				retval += "\" ";
			}
		}
		else throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
	}
	return retval;
}
String::string project::resource()
{
	String::string resourceKey = "resource";
	JSON::object rcArray;
	if (!this->config.contain(resourceKey))
	{
		rcArray = ForeachFiles(this->path, ResourceFilter);
	}
	else
	{
		JSON::object &resource = this->config[resourceKey];
		if (resource.type == JSON::type::STRING)
		{
			String::string folder = this->path + "\\" + resource;
			if (Filesystem::absolute(resource))
			{
				folder = resource;
			}
			rcArray = ForeachFiles(folder, ResourceFilter);
		}
		else if (resource.type == JSON::type::ARRAY)
		{
			QWORD idx = 0;
			for (QWORD i = 0; i < resource.length(); i++)
			{
				JSON::object &resele = resource[i];

				String::string folder = this->path + "\\" + resele;
				if (Filesystem::absolute(resele))
				{
					folder = resele;
				}
				JSON::object rcArr = ForeachFiles(folder, ResourceFilter);
				for (QWORD j = 0; j < rcArr.length(); j++)
				{
					rcArray[idx + j] = rcArr[j];
				}
				idx += rcArr.length();
			}
		}
		else throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
	}

	String::string resFiles;
	for (QWORD i = 0; i < rcArray.length(); i++)
	{
		if (!i)
		{
			Streaming::cout << "\x1B[46;97m RESOURCE \x1B[0m ";
			Streaming::cout << ((String::string) this->config["name"]) << " [" << this->path << "]\n";
		}
		String::string input = rcArray[i];
		String::string output = String::string(input.address, input.length() - 2) + "res";
		String::string cmd = "rc.exe /FO\"";
		cmd += output;
		cmd += "\" ";
		cmd += input;
		if (ExecuteCommand(cmd, this->path))
		{
			Streaming::cout << input << " -> " << output << Streaming::LF;
			resFiles += output;
			resFiles += " ";
		}
	}
	return resFiles;
}
String::string project::source()
{
	String::string sourceKey = "source";
	String::string sourceFiles;
	if (!this->config.contain(sourceKey))
	{
		// No source configured, use project path as source folder
		sourceFiles = ConcatStringArray(ForeachFiles(this->path, SourceFilter));
	}
	else/* if (this->config[sourceKey].type != JSON::type::UNKNOWN)*/
	{
		JSON::object &source = this->config[sourceKey];
		if (source.type == JSON::type::STRING)
		{
			// source in config is a string, only one source folder
			// root path is project path
			String::string folder = this->path + "\\" + source;
			if (Filesystem::absolute(source))
			{
				folder = source;
			}
			sourceFiles = ConcatStringArray(ForeachFiles(folder, SourceFilter));
		}
		else if (source.type == JSON::ARRAY)
		{
			// sources are in an array in config
			for (QWORD i = 0; i < source.length(); i++)
			{
				JSON::object &sourceEle = source[i];

				String::string folder = this->path + "\\" + sourceEle;
				if (Filesystem::absolute(sourceEle))
				{
					folder = sourceEle;
				}
				sourceFiles += ConcatStringArray(ForeachFiles(folder, SourceFilter));
			}
		}
		else if (source.type != JSON::UNKNOWN) throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
	}
	return sourceFiles;
}
String::string project::option(int type)
{
	// opt type, 0 is compiler options, 1 is linker options
	String::string optionsType[] = {"compiler", "linker"};
	String::string optionsKey = "options";
	JSON::object *array = nullptr;
	if (this->config.contain(optionsKey))
	{
		JSON::object &options = this->config[optionsKey];
		if (options.type == JSON::type::COMPONENT)
		{
			// "options": {"compiler": [], "linker": []}
			if (options.contain(optionsType[type]))
			{
				JSON::object &optArr = options[optionsType[type]];
				if (optArr.type == JSON::type::ARRAY)
				{
					array = &optArr;
				}
			}
		}
		else if (options.type == JSON::type::ARRAY)
		{
			// "options": []
			// default is compiler options
			if (type == 0)
			{
				array = &options;
			}
		}
		else throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
	}

	String::string optionsValue;
	if (array)
	{
		optionsValue = ConcatStringArray(*array);
	}
	return optionsValue;
}
String::string project::definitions()
{
	String::string definitionsKey = "definitions";
	if (this->config.contain(definitionsKey))
	{
		JSON::object &definitions = this->config[definitionsKey];
		String::string retval;
		for (QWORD i = 0; i < definitions.length(); i++)
		{
			JSON::object &element = definitions[i];
			retval += "/D ";
			retval += (String::string) element;
			retval += " ";
		}
		return retval;
	}
	return "";
}
// TODO this two function is too similar
String::string project::object()
{
	String::string objKey = "obj";
	if (this->config.contain(objKey))
	{
		JSON::object &obj = this->config[objKey];
		String::string fo = "/Fo\"";
		String::string pathname = EscapeString(obj);
		return fo + pathname + "\" ";
	}
	return "";
}
String::string project::executable()
{
	String::string exeKey = "exe";
	if (this->config.contain(exeKey))
	{
		JSON::object &exe = this->config[exeKey];
		String::string fe = "/Fe:\"";
		String::string pathname = EscapeString(exe);
		return fe + pathname + "\" ";
	}
	return "";
}

int main()
{
	String::string root = Filesystem::canonicalize(".");

	bool success = true;
	try
	{
		// Load project tree
		project *proj = (project *) Memory::allocate(sizeof(project));
		new(proj) project(root);

		// Foreach project list and compile
		ProjectNode *node = ProjectList;
		while (success && node)
		{
			success &= node->project->compile();
			node = node->next;
		}
	}
	catch (Memory::exception &exce)
	{
		success = false;
		Streaming::cout << exce.code << ": " << exce.message << Streaming::LF;
		for (QWORD i = 0; i < exce.count; i++)
		{
			Streaming::cout << exce.stack[i].offset << " (";
			if (exce.stack[i].function.length)
			{
				Streaming::cout << exce.stack[i].function << '+';
				Streaming::cout << Hexadecimal::stringify((QWORD) exce.stack[i].offset - (QWORD) exce.stack[i].address);
			}
			else
			{
				Streaming::cout << exce.stack[i].offset;
			}
			Streaming::cout << ") [";
			if (exce.stack[i].module)
			{
				Streaming::cout << exce.stack[i].library << '+';
				Streaming::cout << Hexadecimal::stringify((QWORD) exce.stack[i].offset - (QWORD) exce.stack[i].module);
			}
			else
			{
				Streaming::cout << exce.stack[i].address;
			}
			Streaming::cout << ']' << Streaming::LF;
		}
	}
	catch (...)
	{
		success = false;
		Streaming::cout << "Unknown internal error" << Streaming::LF;
	}

	try
	{
		FreeList();
	}
	catch (...)
	{
	}

	if (success)
	{
		Streaming::cout << "\x1B[42;97m SUCCESSFUL \x1B[0m";
	}
	else
	{
		Streaming::cout << "\x1B[41;97m FAILED \x1B[0m";
	}
	return 0;
}