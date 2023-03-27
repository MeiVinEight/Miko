#include "definitions.h"

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
			DWORD desiredAccess = GENERIC_ALL;
			DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
			HANDLE h = CreateFileA((LPCSTR) cpath.address, desiredAccess, shareMode, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (h == INVALID_HANDLE_VALUE)
			{
				DWORD err = GetLastError();
				if (err != ERROR_FILE_EXISTS)
				{
					throw Exception::exception(Exception::message(err));
				}
				return false;
			}
			CloseHandle(h);
			return true;
		}
		return false;
	}
	// path is not a file
	char append[] = " is not a file";
	QWORD appendLen = sizeof(append) - 1;
	Memory::string msg(canon.length() + appendLen);
	Memory::copy(msg.address, canon.address.address, canon.length());
	Memory::copy(msg.address + canon.length(), append, appendLen);
	throw Exception::exception(msg);
}
bool Filesystem::make(const String::string &path)
{
	String::string canon = Filesystem::canonicalize(path);
	if (!Filesystem::file(path))
	{
		if (!Filesystem::exist(path))
		{
			Filesystem::make(Filesystem::parent(path));
			if (CreateDirectoryA((LPCSTR) cstring(canon).address, nullptr))
			{
				return true;
			}
			throw Exception::exception(Exception::message(GetLastError()));
		}
		return false;
	}
	// path is not a directory
	char append[] = " is not a directory";
	QWORD appendLen = sizeof(append) - 1;
	Memory::string msg(canon.length() + appendLen);
	Memory::copy(msg.address, canon.address.address, canon.length());
	Memory::copy(msg.address + canon.length(), append, appendLen);
	throw Exception::exception(msg);
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
	if (GetFileAttributesA(cstr) == INVALID_FILE_ATTRIBUTES)
	{
		DWORD err = GetLastError();
		if (err != ERROR_FILE_NOT_FOUND && err != ERROR_PATH_NOT_FOUND)
		{
			throw Exception::exception(Exception::message(err));
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
		DWORD err = GetLastError();
		if (err != ERROR_FILE_NOT_FOUND && err != ERROR_PATH_NOT_FOUND)
		{
			throw Exception::exception(Exception::message(err));
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
	throw Exception::exception(Exception::message(GetLastError()));
}
QWORD Filesystem::open(const String::string &path, DWORD mode)
{
	Filesystem::create(path);
	OFSTRUCT data;
	HFILE hfVal = OpenFile((char *) cstring(path).address, &data, mode);
	if (hfVal == Filesystem::FILE_ERROR)
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return hfVal;
}
void Filesystem::close(QWORD fdVal)
{
	if (!CloseHandle((HANDLE)fdVal))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
}
DWORD Filesystem::read(QWORD fdVal, void *b, DWORD len)
{
	DWORD readed;
	if (!ReadFile((HANDLE)fdVal, b, len, &readed, nullptr))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return readed;
}
DWORD Filesystem::write(QWORD fdVal, const void *b, DWORD len)
{
	DWORD written;
	if (!WriteFile((HANDLE)fdVal, b, len, &written, nullptr))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return written;
}
void Filesystem::seek(QWORD fdVal, QWORD offset, DWORD mode)
{
	LARGE_INTEGER distance;
	distance.QuadPart = (long long) offset;
	if (!SetFilePointerEx((HANDLE)fdVal, distance, nullptr, mode))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
}
void Filesystem::flush(QWORD fdVal)
{
	if (!FlushFileBuffers((HANDLE) fdVal))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
}