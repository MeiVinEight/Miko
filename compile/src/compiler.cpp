#include <memory.h>
#include <sstring.h>
#include <json.h>
#include <streaming.h>
#include <filesystem.h>

/*-------------------------------- NT declarations --------------------------------*/
#pragma region NTDECL
#define MAX_PATH          260

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
	char     cFileName[ MAX_PATH ];
	char     cAlternateFileName[ 14 ];
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
BOOL __stdcall SetConsoleTextAttribute(void *, DWORD);

}
#pragma endregion NTDECL
/*-------------------------------- NT declarations --------------------------------*/

// placement new
void *__cdecl operator new(unsigned long long, void *where) noexcept
{
	return where;
}


// project object
class project
{
	public:
	String::string path;       // project's path
	JSON::object config;       // project's config, JSON object
	bool compiled = false;     // whether project has successfully built in current build task

	project(String::string);
	void prefix();
	bool compile();
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
bool SourceFilter(const char *name)
{
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
// Foreach files in folder and concat them into a string
String::string ForeachFiles(const String::string &folder, bool (*filter)(const char *))
{
	String::string files;
	Memory::string cstr = (folder + "\\*").native();
	WIN32_FIND_DATAA data = {};
	void *handle = FindFirstFileA((const char *) cstr.address, &data);
	if (handle != ((void *) -1))
	{
		bool search = true;
		while (search)
		{
			if (filter(data.cFileName))
			{
				files += folder;
				files += "\\";
				files += data.cFileName;
				files += " ";
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
}


project::project(String::string path): path((String::string &&) path), config(JSON::resolve(ReadJson(this->path + "\\config.json")))
{
	if (this->config.type != JSON::type::COMPONENT)
	{
		Streaming::cout << this->path << Streaming::LF;
		Streaming::cout << "config file must be component" << Streaming::LF;
		throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
	}
	Streaming::cout << "Loading project: " << ((String::string) this->config["name"]) << " [" << this->path << "]\n";
	// Link this project into project list
	// use header link
	ProjectNode *node = (ProjectNode *) Memory::allocate(sizeof(ProjectNode));
	node->project = this;
	node->next = ProjectList;
	ProjectList = node;

	// load subprojects if it has "subproject" config field
	static String::string subprojKey = "subproject";
	if (this->config.contain(subprojKey))
	{
		JSON::object subprojects = this->config[subprojKey];
		if (subprojects.type != JSON::type::ARRAY)
		{
			Streaming::cout << "subproject must be array" << Streaming::LF;
			return;
		}

		for (QWORD i = 0; i < subprojects.length(); i++)
		{
			JSON::object subproj = subprojects[i];
			if (subproj.type != JSON::type::STRING)
			{
				Streaming::cout << "subproject element must be string" << Streaming::LF;
			}
			else
			{
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
}
void project::prefix()
{
	Streaming::cout << "[" << ((String::string &) this->config["name"]) << "]: ";
}
bool project::compile()
{
	// This project never compiled in current build task
	if (this->compiled)
		return true;

	bool success = true;
	// compile depends
	static String::string dependsKey = "depends";
	if (this->config.contain(dependsKey))
	{
		JSON::object &depends = this->config[dependsKey];
		if (depends.type != JSON::type::ARRAY)
		{
			this->prefix();
			Streaming::cout << "depends must be array" << Streaming::LF;
		}
		else
		{
			for (QWORD i = 0; success && i < depends.size; i++)
			{
				JSON::object &depele = depends[i];
				if (depele.type != JSON::type::STRING)
				{
					this->prefix();
					Streaming::cout << "depends element must be string" << Streaming::LF;
				}
				else
				{
					const String::string &depName = depele;
					project *depproj = FindProject(depName);
					if (depproj)
					{
						success &= depproj->compile();
					}
					else
					{
						this->prefix();
						Streaming::cout << "Cannot find project [" << depName << "]\n";
					}
				}
			}
		}
	}
	if (!success)
		return false;
	// String::string &name = this->config["name"];
	String::string sourceKey = "source";

	// default compiler is cl.exe
	String::string clr = "cl.exe ";



	String::string sourceFiles;
	if (!this->config.contain(sourceKey))
	{
		// Not source configured, use project path as source folder
		sourceFiles = ForeachFiles(this->path, SourceFilter);
		/*
		Memory::string cstr = this->path.native();
		WIN32_FIND_DATAA data = {};
		void *handle = FindFirstFileA((const char *) cstr.address, &data);
		if (handle != ((void *) -1))
		{
			bool search = true;
			while (search)
			{
				sourceFiles += data.cFileName;
				sourceFiles += " ";
				search = FindNextFileA(handle, &data);
			}
			FindClose(handle);
		}
		*/
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
			sourceFiles = ForeachFiles(folder, SourceFilter);
		}
		else if (source.type == JSON::type::ARRAY)
		{
			// sources are in an array in config
			for (QWORD i = 0; i < source.size; i++)
			{
				JSON::object &sourceEle = source[i];
				if (sourceEle.type != JSON::type::STRING)
				{
					this->prefix();
					Streaming::cout << "source element must be string" << Streaming::LF;
				}
				else
				{
					String::string folder = this->path + "\\" + sourceEle;
					if (Filesystem::absolute(sourceEle))
					{
						folder = sourceEle;
					}
					sourceFiles += ForeachFiles(folder, SourceFilter);
				}
			}
		}
		else if (source.type != JSON::type::UNKNOWN)
		{
			// configured source bug unrecognized
			this->prefix();
			Streaming::cout << "Connot find source folders" << Streaming::LF;
			return false;
		}
	}

	// if project has source file(s), compile and link them
	if (sourceFiles)
	{
		String::string clropt;
		String::string optionKey = "option";
		// check compiler option
		// $.option
		if (this->config.contain(optionKey))
		{
			JSON::object &options = this->config[optionKey];
			if (options.type != JSON::type::ARRAY)
			{
				this->prefix();
				Streaming::cout << "option must be array" << Streaming::LF;
			}
			else
			{
				for (QWORD i = 0; i < options.length(); i++)
				{
					if (options[i].type != JSON::type::STRING)
					{
						this->prefix();
						Streaming::cout << "option element must be string" << Streaming::LF;
					}
					else
					{
						clropt += options[i];
						clropt += " ";
					}
				}
			}
		}


		this->prefix();
		Streaming::cout << "Compile project" << Streaming::LF;
		success &= ExecuteCommand(clr + sourceFiles + clropt, this->path);
		Streaming::cout << Streaming::LF;
		if (!success)
			return false;
	}
	this->compiled = true;
	return true;
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
		while (node)
		{
			success &= node->project->compile();
			node = node->next;
		}

		FreeList();
	}
	catch (Memory::exception &exce)
	{
		Streaming::cout << exce.message << Streaming::LF;
		success = false;
		try
		{
			FreeList();
		}
		catch (...)
		{
		}
	}
	catch (...)
	{
	}

	if (success)
	{
		SetConsoleTextAttribute((void *) Filesystem::STDOUT, 0x2F);
		Streaming::cout << " SUCCESSFUL ";
		SetConsoleTextAttribute((void *) Filesystem::STDOUT, 0x07);
	}
	else
	{
		SetConsoleTextAttribute((void *) Filesystem::STDOUT, 0x4F);
		Streaming::cout << " FAILED ";
		SetConsoleTextAttribute((void *) Filesystem::STDOUT, 0x07);
	}
	return 0;
}