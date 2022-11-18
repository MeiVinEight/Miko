#include "fsdef.h"

bool FileSystem::create(const void *path)
{
	DWORD desiredAccess = GENERIC_ALL;
	DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	HANDLE h = CreateFileA((LPCSTR)path, desiredAccess, shareMode, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
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

bool FileSystem::exist(const void *path)
{
	OFSTRUCT data;
	data.cBytes = sizeof(OFSTRUCT);
	return !!(~OpenFile((LPCSTR)path, &data, OF_EXIST));
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
	// TODO UNC ?
	char buf[MAX_PATH + 1];
	GetFullPathNameA((LPCSTR) path, MAX_PATH + 1, buf, NULL);
	QWORD len = strlen(buf);
	Memory::string canon(len + 1);
	canon[len] = 0;
	Memory::copy(canon.address, buf, len);
	return canon;
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