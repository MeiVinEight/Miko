#include "definitions.h"

bool Filesystem::create(const void *path)
{
	Memory::string canon = Filesystem::canonicalize(path);
	if (!Filesystem::directory(path))
	{
		if (!Filesystem::exist(path))
		{
			Filesystem::make(Filesystem::parent(path));
			DWORD desiredAccess = GENERIC_ALL;
			DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
			HANDLE h = CreateFileA((LPCSTR) path, desiredAccess, shareMode, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
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
	Memory::string msg(canon.length + sizeof(append) - 1);
	Memory::copy(msg, canon, canon.length - 1);
	Memory::copy(msg + (canon.length - 1), append, sizeof(append));
	msg[msg.length - 1] = 0;
	throw Exception::exception(msg);
}

bool Filesystem::make(const void *path)
{
	Memory::string canon = Filesystem::canonicalize(path);
	if (!Filesystem::file(path))
	{
		if (!Filesystem::exist(path))
		{
			Filesystem::make(Filesystem::parent(path));
			if (CreateDirectoryA(canon, NULL))
			{
				return true;
			}
			throw Exception::exception(Exception::message(GetLastError()));
		}
		return false;
	}
	// path is not a directory
	char append[] = " is not a directory";
	Memory::string msg(canon.length + sizeof(append) - 1);
	Memory::copy(msg, canon, canon.length - 1);
	Memory::copy(msg + (canon.length - 1), append, sizeof(append));
	msg[msg.length - 1] = 0;
	throw Exception::exception(msg);
}

bool Filesystem::exist(const void *path)
{
	if (GetFileAttributesA((LPCSTR)path) == INVALID_FILE_ATTRIBUTES)
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

bool Filesystem::file(const void *path)
{
	return Filesystem::exist(path) && !Filesystem::directory(path);
}

bool Filesystem::directory(const void *path)
{
	WIN32_FILE_ATTRIBUTE_DATA data{0};
	if (!GetFileAttributesExA((LPCSTR)path, GetFileExInfoStandard, &data))
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

Memory::string Filesystem::parent(const void *path)
{
	Memory::string canon = Filesystem::canonicalize(path);
	QWORD length = canon.length - 1;
	while (length && *(canon + --length) != '\\');
	length = (canon.length == 4) ? 0 : length;
	length = (length && (length < 3)) ? 3 : length;
	Memory::string ret(length + 1);
	Memory::copy(ret, canon, length);
	ret[length] = 0;
	return ret;
}

bool Filesystem::remove(const void *path)
{
	SetFileAttributesA((LPCSTR)path, FILE_ATTRIBUTE_NORMAL);
	DWORD attr = GetFileAttributesA((LPCSTR)path);
	if (~attr)
	{
		if (attr & FILE_ATTRIBUTE_DIRECTORY)
		{
			return RemoveDirectoryA((LPCSTR)path);
		}
		return DeleteFileA((LPCSTR)path);
	}
	return false;
}

Memory::string Filesystem::canonicalize(const void *path) // Maybe only GetFullPathName
{
	char buf[MAX_PATH + 1];
	QWORD len = GetFullPathNameA((LPCSTR) path, MAX_PATH + 1, buf, NULL);
	if (len)
	{
		Memory::string canon(len + 1);
		canon[len] = 0;
		Memory::copy(canon, buf, len);
		return canon;
	}
	throw Exception::exception(Exception::message(GetLastError()));
}

Filesystem::FD Filesystem::open(const void *path, DWORD mode)
{
	Filesystem::create(path);
	OFSTRUCT data;
	HFILE hfVal = OpenFile((LPCSTR)path, &data, mode);
	if (hfVal == HFILE_ERROR)
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return hfVal;
}

void Filesystem::close(Filesystem::FD fdVal)
{
	if (!CloseHandle((HANDLE)fdVal))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
}

DWORD Filesystem::read(Filesystem::FD fdVal, void *b, DWORD len)
{
	DWORD readed;
	if (!ReadFile((HANDLE)fdVal, b, len, &readed, NULL))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return readed;
}

DWORD Filesystem::write(Filesystem::FD fdVal, void *b, DWORD len)
{
	DWORD written;
	if (!WriteFile((HANDLE)fdVal, b, len, &written, NULL))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return written;
}

void Filesystem::seek(Filesystem::FD fdVal, QWORD offset, DWORD mode)
{
	LARGE_INTEGER distance;
	distance.QuadPart = (long long) offset;
	if (!SetFilePointerEx((HANDLE)fdVal, distance, NULL, mode))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
}