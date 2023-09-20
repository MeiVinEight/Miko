#include <foreach_file.h>
#include <exception.h>

#include "definitions.h"

#pragma region STRUCT
typedef struct
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
#pragma endregion

#pragma region FUNCTION
extern "C"
{
void *__stdcall FindFirstFileA(const char *, LPWIN32_FIND_DATAA);
int __stdcall FindNextFileA(void *, LPWIN32_FIND_DATAA);
int __stdcall FindClose(void *);
}
#pragma endregion

Filesystem::foreach::~foreach()
{
	this->close();
}
bool Filesystem::foreach::operator()(const String::string &folder)
{
	WIN32_FIND_DATAA data{};
	Memory::string path = (folder + "\\*").native();
	QWORD obj = (QWORD) FindFirstFileA((const char *) path.address, &data);
	if (~obj)
	{
		this->object = obj;
		this->name = data.cFileName;
		return true;
	}
	return false;
}
bool Filesystem::foreach::operator()()
{
	int result = 0;
	if (this->object && ~this->object)
	{
		WIN32_FIND_DATAA data{};
		result = FindNextFileA((void *) this->object, &data);
		if (!result)
		{
			data.cFileName[0] = 0;
		}
		this->name = data.cFileName;
	}
	return result;
}
bool Filesystem::foreach::first(const String::string &folder)
{
	return (*this)(folder);
}
bool Filesystem::foreach::next()
{
	return (*this)();
}
void Filesystem::foreach::close()
{
	if (this->object && ~this->object)
	{
		FindClose((void *) this->object);
		this->object = 0;
	}
}