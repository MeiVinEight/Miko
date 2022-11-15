#define FS_SHARED

#include <filesystem.h>
#include <exception.h>
#include <memory.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define WINAPI						__stdcall
#define NULL						0
#define INVALID_HANDLE_VALUE		((HANDLE)(LONG_PTR)-1)
#define INVALID_FILE_ATTRIBUTES		(-1)

#define GENERIC_READ				(0x80000000L)
#define GENERIC_WRITE				(0x40000000L)
#define GENERIC_EXECUTE				(0x20000000L)
#define GENERIC_ALL					(0x10000000L)

#define FILE_SHARE_READ				0x00000001
#define FILE_SHARE_WRITE			0x00000002
#define FILE_SHARE_DELETE			0x00000004

#define CREATE_NEW					1
#define FILE_ATTRIBUTE_DIRECTORY	0x00000010
#define FILE_ATTRIBUTE_NORMAL		0x00000080

#define OF_EXIST					0x00004000
#define OF_READWRITE				0x00000002

#define OFS_MAXPATHNAME				128

#define FILE_BEGIN					0
#define FILE_CURRENT				1

#define FILE_TYPE_DISK				0x0001
#define FILE_TYPE_CHAR				0x0002
#define FILE_TYPE_PIPE				0x0003

#define STD_INPUT_HANDLE			((DWORD)-10)

#define ERROR_FILE_EXISTS			80L
#define ERROR_BROKEN_PIPE			109L

typedef const char	*LPCSTR;
typedef void		*HANDLE, *PVOID, *LPVOID;
typedef const void	*LPCVOID;
typedef DWORD		*LPDWORD;
typedef int				BOOL;
typedef char			CHAR;
typedef unsigned int	UINT;
typedef long			LONG;
typedef short			SHORT;
typedef wchar_t			WCHAR;
typedef __int64 			LONG_PTR, LONGLONG;
typedef unsigned __int64	ULONG_PTR;
typedef struct
{
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} *LPSECURITY_ATTRIBUTES;
typedef struct
{
	BYTE cBytes;
	BYTE fFixedDisk;
	WORD nErrCode;
	WORD Reserved1;
	WORD Reserved2;
	CHAR szPathName[OFS_MAXPATHNAME];
} OFSTRUCT, *LPOFSTRUCT;
typedef struct
{
	ULONG_PTR Internal;
	ULONG_PTR InternalHigh;
	union
	{
		struct
		{
			DWORD Offset;
			DWORD OffsetHigh;
		};
		PVOID Pointer;
	};
	HANDLE  hEvent;
} *LPOVERLAPPED;
typedef union
{
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	};
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
typedef struct
{
	SHORT X;
	SHORT Y;
} COORD;
typedef struct
{
	BOOL bKeyDown;
	WORD wRepeatCount;
	WORD wVirtualKeyCode;
	WORD wVirtualScanCode;
	union
	{
		WCHAR UnicodeChar;
		CHAR   AsciiChar;
	} uChar;
	DWORD dwControlKeyState;
} KEY_EVENT_RECORD;
typedef struct
{
	COORD dwMousePosition;
	DWORD dwButtonState;
	DWORD dwControlKeyState;
	DWORD dwEventFlags;
} MOUSE_EVENT_RECORD;
typedef struct
{
	COORD dwSize;
} WINDOW_BUFFER_SIZE_RECORD;
typedef struct
{
	UINT dwCommandId;
} MENU_EVENT_RECORD;
typedef struct
{
	BOOL bSetFocus;
} FOCUS_EVENT_RECORD;
typedef struct
{
	WORD EventType;
	union
	{
		KEY_EVENT_RECORD KeyEvent;
		MOUSE_EVENT_RECORD MouseEvent;
		WINDOW_BUFFER_SIZE_RECORD WindowBufferSizeEvent;
		MENU_EVENT_RECORD MenuEvent;
		FOCUS_EVENT_RECORD FocusEvent;
	} Event;
} INPUT_RECORD, *PINPUT_RECORD;

HANDLE WINAPI CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
HANDLE (WINAPI *CreateFile)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE) = &CreateFileA;
BOOL WINAPI CloseHandle(HANDLE);
HFILE WINAPI OpenFile(LPCSTR, LPOFSTRUCT, UINT);
DWORD WINAPI GetLastError(void);
BOOL WINAPI ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
BOOL WINAPI WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
BOOL WINAPI SetFilePointerEx(HANDLE, LARGE_INTEGER, PLARGE_INTEGER, DWORD);
DWORD WINAPI GetFileType(HANDLE);
HANDLE WINAPI GetStdHandle(DWORD);
BOOL WINAPI GetNumberOfConsoleInputEvents(HANDLE, LPDWORD);
BOOL WINAPI PeekConsoleInputA(HANDLE, PINPUT_RECORD, DWORD, LPDWORD);
BOOL (WINAPI *PeekConsoleInput)(HANDLE, PINPUT_RECORD, DWORD, LPDWORD) = &PeekConsoleInputA;
BOOL WINAPI PeekNamedPipe(HANDLE, LPVOID, DWORD, LPDWORD, LPDWORD, LPDWORD);
BOOL WINAPI GetFileSizeEx(HANDLE, PLARGE_INTEGER);
BOOL WINAPI SetFileAttributesA(LPCSTR, DWORD);
DWORD WINAPI GetFileAttributesA(LPCSTR);
BOOL WINAPI RemoveDirectoryA(LPCSTR);
BOOL WINAPI DeleteFileA(LPCSTR);

#ifdef __cplusplus
}
#endif


bool FileSystem::create(const void *path)
{
	DWORD desiredAccess = GENERIC_ALL;
	DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	HANDLE h = CreateFile((LPCSTR)path, desiredAccess, shareMode, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		if (err != ERROR_FILE_EXISTS)
		{
			throw Exception::exception(Exception::exception::INTERNAL, err);
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

FileSystem::FD FileSystem::open(const void *path, DWORD mode)
{
	FileSystem::create(path);
	OFSTRUCT data;
	HFILE hfVal = OpenFile((LPCSTR)path, &data, mode);
	if (hfVal == HFILE_ERROR)
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
	return hfVal;
}

void FileSystem::close(FileSystem::FD fdVal)
{
	if (!CloseHandle((HANDLE)fdVal))
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
}

DWORD FileSystem::read(FileSystem::FD fdVal, void *b, DWORD len)
{
	DWORD readed;
	if (!ReadFile((HANDLE)fdVal, b, len, &readed, NULL))
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
	return readed;
}

DWORD FileSystem::write(FileSystem::FD fdVal, void *b, DWORD len)
{
	DWORD written;
	if (!WriteFile((HANDLE)fdVal, b, len, &written, NULL))
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
	return written;
}

void FileSystem::seek(FileSystem::FD fdVal, QWORD offset, DWORD mode)
{
	LARGE_INTEGER distance;
	distance.QuadPart = (long long) offset;
	if (!SetFilePointerEx((HANDLE)fdVal, distance, NULL, mode))
	{
		throw Exception::exception(Exception::exception::EXTERNAL, GetLastError());
	}
}

FileSystem::File::File(const void *path): file(FileSystem::open(path, OF_READWRITE))
{
}

FileSystem::File::File(FileSystem::FD fdVal): file(fdVal)
{
}

FileSystem::File::File(FileSystem::File &&another) noexcept: file(another.file)
{
	if (~this->file)
	{
		CloseHandle((HANDLE) this->file);
	}
	this->file = another.file;
	another.file = HFILE_ERROR;
}

FileSystem::File::~File()
{
	this->close();
}

FileSystem::File &FileSystem::File::operator=(FileSystem::File &&fVal) noexcept
{
	if (this->file)
	{
		CloseHandle((HANDLE)this->file);
	}
	this->file = fVal.file;
	fVal.file = HFILE_ERROR;
	return *this;
}

DWORD FileSystem::File::read(void *b, DWORD len)
{
	if (~this->file)
	{
		return FileSystem::read(this->file, b, len);
	}
	throw Exception::exception(Exception::exception::EXTERNAL, FileSystem::FILE_CLOSED);
}

DWORD FileSystem::File::write(void *b, DWORD len)
{
	if (~this->file)
	{
		return FileSystem::write(this->file, b, len);
	}
	throw Exception::exception(Exception::exception::EXTERNAL, FileSystem::FILE_CLOSED);
}

QWORD FileSystem::File::available()
{
	if (~this->file)
	{
		HANDLE handle = (HANDLE) this->file;
		DWORD type = GetFileType(handle);
		switch (type)
		{
			case FILE_TYPE_CHAR:
			case FILE_TYPE_PIPE:
			{
				HANDLE stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
				if (stdInHandle == handle)
				{
					BOOL noerr;
					DWORD events;
					noerr = GetNumberOfConsoleInputEvents(stdInHandle, &events);
					if (noerr)
					{
						DWORD bufferSize = events + sizeof(INPUT_RECORD);
						bufferSize = bufferSize ? bufferSize : 1;
						INPUT_RECORD *lpBuffer = (INPUT_RECORD *) Memory::allocate(bufferSize);
						if (lpBuffer)
						{
							DWORD eventsRead;
							noerr = PeekConsoleInput(handle, lpBuffer, events, &eventsRead);
							if (noerr)
							{
								DWORD curLength = 0;
								DWORD actLength = 0;
								for (DWORD i = 0; i < events; i++)
								{
									if (lpBuffer[i].EventType == 1)
									{
										KEY_EVENT_RECORD *keyRecord = (KEY_EVENT_RECORD *) &(lpBuffer[i].Event);
										if (keyRecord->bKeyDown == 1)
										{
											CHAR *keyPressed = (CHAR *) &(keyRecord->uChar);
											curLength++;
											if (*keyPressed == '\r')
											{
												actLength = curLength;
											}
										}
									}
								}
								Memory::free(lpBuffer);
								return actLength;
							}
							Memory::free(lpBuffer);
						}
						break;
					}
					goto NOSEEK;
				}
				else
				{
					NOSEEK:;
					DWORD ret = 0;
					if (!PeekNamedPipe(handle, NULL, 0, NULL, &ret, NULL))
					{
						DWORD err = GetLastError();
						if (err != ERROR_BROKEN_PIPE)
						{
							goto AVA_ERROR;
						}
						ret = 0;
					}
					return ret;
				}
			}
			case FILE_TYPE_DISK:
			{
				LARGE_INTEGER distance, pos;
				distance.QuadPart = 0;
				if (SetFilePointerEx(handle, distance, &pos, FILE_CURRENT))
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
	}
	AVA_ERROR:
	return -1;
}

void FileSystem::File::seek(QWORD offset) const
{
	FileSystem::seek(this->file, offset, FILE_BEGIN);
}

void FileSystem::File::close()
{
	if (~this->file)
	{
		FileSystem::close(this->file);
		this->file = HFILE_ERROR;
	}
}