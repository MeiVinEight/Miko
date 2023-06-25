#include <exception.h>

#include "definitions.h"

#pragma region STRUCT
typedef struct
{
	BYTE cBytes;
	BYTE fFixedDisk;
	WORD nErrCode;
	WORD Reserved1;
	WORD Reserved2;
	char szPathName[128];
} OFSTRUCT;
typedef union
{
	struct
	{
		DWORD LowPart;
		long HighPart;
	};
	struct
	{
		DWORD LowPart;
		long HighPart;
	} u;
	long long QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
typedef struct
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef struct
{
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA;
typedef enum
{
	GetFileExInfoStandard,
	GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;
#pragma endregion

#pragma region KERNEL
extern "C"
{
void *__stdcall CreateFileA(const char *, DWORD, DWORD, void *, DWORD, DWORD, void *);
int __stdcall OpenFile(const char *, void *, unsigned int);
BOOL __stdcall ReadFile(void *, void *, DWORD, DWORD *, void *);
BOOL __stdcall WriteFile(void *, const void *, DWORD, DWORD *, void *);
BOOL __stdcall SetFilePointerEx(void *, LARGE_INTEGER, PLARGE_INTEGER, DWORD);
DWORD __stdcall GetFileType(void *);
BOOL __stdcall SetFileAttributesA(const char *, DWORD);
DWORD __stdcall GetFileAttributesA(const char *);
BOOL __stdcall RemoveDirectoryA(const char *);
BOOL __stdcall DeleteFileA(const char *);
DWORD __stdcall GetFullPathNameA(const char *, DWORD, char *, char **);
BOOL __stdcall GetFileAttributesExA(const char *, GET_FILEEX_INFO_LEVELS, void *);
BOOL __stdcall CreateDirectoryA(const char *, void *);
BOOL __stdcall FlushFileBuffers(void *);
void *__stdcall GetStdHandle(DWORD);
BOOL __stdcall GetFileSizeEx(void *, PLARGE_INTEGER);
BOOL __stdcall CloseHandle(void *);
}
#pragma endregion

const DWORD Filesystem::ERRNO_WRONG_FILE_TYPE = Memory::registry("Wrong file type");
const QWORD Filesystem::STDIN = (QWORD) GetStdHandle(STD_INPUT_HANDLE);
const QWORD Filesystem::STDOUT = (QWORD) GetStdHandle(STD_OUTPUT_HANDLE);

bool Filesystem::create(const String::string &path)
{
	String::string canon = Filesystem::canonicalize(path);
	if (!Filesystem::directory(path))
	{
		if (!Filesystem::exist(path))
		{
			Filesystem::make(Filesystem::parent(path));
			Memory::string cpath = cstring(path);
			void *h = CreateFileA((const char *) cpath.address, 0x10000000L, 7, nullptr, 1, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (h == (void *) ~((QWORD) 0))
			{
				DWORD err = Memory::error();
				if (err != ERROR_FILE_EXISTS)
				{
					throw Memory::exception(err, Memory::DOSERROR);
				}
				return false;
			}
			CloseHandle(h);
			return true;
		}
		return false;
	}
	// path is not a file
	throw Memory::exception(Filesystem::ERRNO_WRONG_FILE_TYPE);
}
bool Filesystem::make(const String::string &path)
{
	String::string canon = Filesystem::canonicalize(path);
	if (!Filesystem::file(path))
	{
		if (!Filesystem::exist(path))
		{
			Filesystem::make(Filesystem::parent(path));
			if (CreateDirectoryA((const char *) cstring(canon).address, nullptr))
			{
				return true;
			}
			throw Memory::exception(Memory::error(), Memory::DOSERROR);
		}
		return false;
	}
	// path is not a directory
	throw Memory::exception(Filesystem::ERRNO_WRONG_FILE_TYPE);
}
bool Filesystem::remove(const String::string &path)
{
	Memory::string cpath = cstring(path);
	char *cstr = (char *) cpath.address;
	SetFileAttributesA(cstr, FILE_ATTRIBUTE_NORMAL);
	DWORD attr = GetFileAttributesA(cstr);
	if (~attr)
	{
		if (attr & FILE_ATTRIBUTE_DIRECTORY)
		{
			return RemoveDirectoryA(cstr);
		}
		return DeleteFileA(cstr);
	}
	return false;
}
bool Filesystem::exist(const String::string &path)
{
	Memory::string cpath = cstring(path);
	char *cstr = (char *) cpath.address;
	if (GetFileAttributesA(cstr) == -1)
	{
		DWORD err = Memory::error();
		if (err != ERROR_FILE_NOT_FOUND && err != ERROR_PATH_NOT_FOUND)
		{
			throw Memory::exception(err, Memory::DOSERROR);
		}
		return false;
	}
	return true;
}
bool Filesystem::file(const String::string &path)
{
	return Filesystem::exist(path) && !Filesystem::directory(path);
}
bool Filesystem::directory(const String::string &path)
{
	WIN32_FILE_ATTRIBUTE_DATA data{0};
	if (!GetFileAttributesExA((char *) cstring(path).address, GetFileExInfoStandard, &data))
	{
		DWORD err = Memory::error();
		if (err != ERROR_FILE_NOT_FOUND && err != ERROR_PATH_NOT_FOUND)
		{
			throw Memory::exception(err, Memory::DOSERROR);
		}
		return false;
	}
	return !!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}
Memory::string Filesystem::parent(const String::string &path)
{
	String::string canon = Filesystem::canonicalize(path);
	QWORD length = canon.length();
	while (length && *(canon.address.address + --length) != '\\');
	length = (canon.length() == 3) ? 0 : length;
	length = (length && (length < 3)) ? 3 : length;
	Memory::string ret(length);
	Memory::copy(ret.address, canon.address.address, length);
	return ret;
}
Memory::string Filesystem::canonicalize(const String::string &path)
{
	// Maybe only GetFullPathName
	char buf[MAX_PATH + 1];
	QWORD len = GetFullPathNameA((char *) cstring(path).address, MAX_PATH + 1, buf, nullptr);
	if (len)
	{
		Memory::string canon(len);
		Memory::copy(canon.address, buf, len);
		return canon;
	}
	throw Memory::exception(Memory::error(), Memory::DOSERROR);
}
bool Filesystem::absolute(const String::string &path)
{
	return path.length() >= 2 && path[1] == ':';
}
QWORD Filesystem::open(const String::string &path, DWORD mode)
{
	OFSTRUCT data;
	int hfVal = OpenFile((char *) cstring(path).address, &data, mode);
	if (hfVal == Filesystem::FILE_ERROR)
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
	return hfVal;
}
void Filesystem::close(QWORD fdVal)
{
	if (!CloseHandle((void *) fdVal))
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
}
DWORD Filesystem::read(QWORD fdVal, void *b, DWORD len)
{
	DWORD readed;
	if (!ReadFile((void *) fdVal, b, len, &readed, nullptr))
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
	return readed;
}
DWORD Filesystem::write(QWORD fdVal, const void *b, DWORD len)
{
	DWORD written;
	if (!WriteFile((void *) fdVal, b, len, &written, nullptr))
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
	return written;
}
void Filesystem::seek(QWORD fdVal, QWORD offset, DWORD mode)
{
	LARGE_INTEGER distance;
	distance.QuadPart = (long long) offset;
	if (!SetFilePointerEx((void *) fdVal, distance, nullptr, mode))
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
}
void Filesystem::flush(QWORD fdVal)
{
	if (!FlushFileBuffers((void *) fdVal))
	{
		throw Memory::exception(Memory::error(), Memory::DOSERROR);
	}
}
QWORD Filesystem::available(QWORD fdVal)
{
	void * handle = (void *) fdVal;
	DWORD type = GetFileType(handle);
	switch (type)
	{
		case FILE_TYPE_DISK:
		{
			LARGE_INTEGER distance, pos;
			distance.QuadPart = 0;
			if (SetFilePointerEx(handle, distance, &pos, Filesystem::SEEK_CURRENT))
			{
				QWORD current = pos.QuadPart;
				LARGE_INTEGER filesize;
				if (GetFileSizeEx(handle, &filesize))
				{
					return filesize.QuadPart - current;
				}
			}
			break;
		}
	}
	return -1;
}