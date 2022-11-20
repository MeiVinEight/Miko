#include "fsdef.h"

bool FileSystem::create(const void *path)
{
	Memory::string canon = FileSystem::canonicalize(path);
	if (!FileSystem::directory(path))
	{
		if (!FileSystem::exist(path))
		{
			FileSystem::make(FileSystem::parent(path));
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
	// path is not a directory
	char append[] = " is not a file";
	Memory::string msg(canon.length + sizeof(append) - 1);
	Memory::copy(msg, canon, canon.length - 1);
	Memory::copy(msg + (canon.length - 1), append, sizeof(append));
	msg[msg.length - 1] = 0;
	throw Exception::exception(msg);
}

bool FileSystem::make(const void *path)
{
	Memory::string canon = FileSystem::canonicalize(path);
	if (!FileSystem::file(path))
	{
		if (!FileSystem::exist(path))
		{
			FileSystem::make(FileSystem::parent(path));
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

bool FileSystem::exist(const void *path)
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

bool FileSystem::file(const void *path)
{
	return FileSystem::exist(path) && !FileSystem::directory(path);
}

bool FileSystem::directory(const void *path)
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

Memory::string FileSystem::parent(const void *path)
{
	Memory::string canon = FileSystem::canonicalize(path);
	QWORD length = canon.length - 1;
	while (length && *(canon + --length) != '\\');
	length = (canon.length == 4) ? 0 : length;
	length = (length && (length < 3)) ? 3 : length;
	Memory::string ret(length + 1);
	Memory::copy(ret, canon, length);
	ret[length] = 0;
	return ret;
}

bool FileSystem::remove(const void *path)
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

Memory::string FileSystem::canonicalize(const void *path) // Maybe only GetFullPathName
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

FileSystem::FD FileSystem::open(const void *path, DWORD mode)
{
	FileSystem::create(path);
	OFSTRUCT data;
	HFILE hfVal = OpenFile((LPCSTR)path, &data, mode);
	if (hfVal == HFILE_ERROR)
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return hfVal;
}

void FileSystem::close(FileSystem::FD fdVal)
{
	if (!CloseHandle((HANDLE)fdVal))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
}

DWORD FileSystem::read(FileSystem::FD fdVal, void *b, DWORD len)
{
	DWORD readed;
	if (!ReadFile((HANDLE)fdVal, b, len, &readed, NULL))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return readed;
}

DWORD FileSystem::write(FileSystem::FD fdVal, void *b, DWORD len)
{
	DWORD written;
	if (!WriteFile((HANDLE)fdVal, b, len, &written, NULL))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
	return written;
}

void FileSystem::seek(FileSystem::FD fdVal, QWORD offset, DWORD mode)
{
	LARGE_INTEGER distance;
	distance.QuadPart = (long long) offset;
	if (!SetFilePointerEx((HANDLE)fdVal, distance, NULL, mode))
	{
		throw Exception::exception(Exception::message(GetLastError()));
	}
}