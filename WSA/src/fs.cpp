#define WSA_SHARED

#include <fs.h>
#include <exception.h>
#include <memory.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define CONST					const
#define WINAPI					__stdcall
#define NULL					0
#define GENERIC_READ			(0x80000000L)
#define GENERIC_WRITE			(0x40000000L)
#define FILE_SHARE_READ			0x00000001
#define FILE_SHARE_WRITE		0x00000002
#define CREATE_NEW				1
#define FILE_ATTRIBUTE_NORMAL	0x00000080
#define INVALID_HANDLE_VALUE	((HANDLE)(LONG_PTR)-1)
#define OF_EXIST				0x00004000
#define OF_READWRITE			0x00000002
#define OFS_MAXPATHNAME			128
#define FILE_BEGIN				0
#define FILE_TYPE_DISK			0x0001
#define FILE_TYPE_CHAR			0x0002
#define FILE_TYPE_PIPE			0x0003
#define STD_INPUT_HANDLE		((DWORD)-10)
#define ERROR_BROKEN_PIPE		109L
#define FILE_CURRENT			1

typedef void		*HANDLE;
typedef void		*PVOID;
typedef void		*LPVOID;
typedef CONST void	*LPCVOID;
typedef DWORD		*LPDWORD;
typedef char			CHAR;
typedef unsigned int	UINT;
typedef long			LONG;
typedef short			SHORT;
typedef wchar_t			WCHAR;
typedef __int64 			LONG_PTR;
typedef __int64				LONGLONG;
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

#ifdef __cplusplus
}
#endif


BOOL WSA::create(LPCSTR path)
{
	HANDLE h = CreateFile(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	BYTE success = h != INVALID_HANDLE_VALUE;
	if (success)
	{
		CloseHandle(h);
	}
	return success;
}

BYTE WSA::exist(LPCSTR path)
{
	OFSTRUCT data;
	if (OpenFile(path, &data, OF_EXIST) == HFILE_ERROR)
	{
		return 0;
	}
	return 1;
}

WSA::FD WSA::open(LPCSTR path, DWORD mode)
{
	WSA::create(path);
	OFSTRUCT data;
	HFILE hfVal = OpenFile(path, &data, mode);
	if (hfVal == HFILE_ERROR)
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
	return hfVal;
}

void WSA::close(WSA::FD fdVal)
{
	if (!CloseHandle((HANDLE)fdVal))
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
}

DWORD WSA::read(WSA::FD fdVal, BYTE *b, DWORD len)
{
	DWORD readed;
	if (!ReadFile((HANDLE)fdVal, b, len, &readed, NULL))
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
	return readed;
}

DWORD WSA::write(WSA::FD fdVal, BYTE *b, DWORD len)
{
	DWORD written;
	if (!WriteFile((HANDLE)fdVal, b, len, &written, NULL))
	{
		throw Exception::exception(Exception::exception::INTERNAL, GetLastError());
	}
	return written;
}

void WSA::seek(WSA::FD fdVal, QWORD offset, DWORD mode)
{
	LARGE_INTEGER distance;
	distance.QuadPart = (long long) offset;
	if (!SetFilePointerEx((HANDLE)fdVal, distance, NULL, mode))
	{
		throw Exception::exception(Exception::exception::EXTERNAL, GetLastError());
	}
}

WSA::FIO::FIO(LPCSTR path): file(WSA::open(path, OF_READWRITE))
{
}

WSA::FIO::FIO(WSA::FD fdVal): file(fdVal)
{
}

WSA::FIO::FIO(WSA::FIO &&another) noexcept : file(another.file)
{
	if (~this->file)
	{
		CloseHandle((HANDLE) this->file);
	}
	this->file = another.file;
	another.file = HFILE_ERROR;
}

WSA::FIO::~FIO()
{
	this->close();
}

WSA::FIO &WSA::FIO::operator=(WSA::FIO &&fVal) noexcept
{
	if (this->file)
	{
		CloseHandle((HANDLE)this->file);
	}
	this->file = fVal.file;
	fVal.file = HFILE_ERROR;
	return *this;
}

DWORD WSA::FIO::read(BYTE *b, DWORD len)
{
	if (~this->file)
	{
		return WSA::read(this->file, b, len);
	}
	throw Exception::exception(Exception::exception::EXTERNAL, WSA::FILE_CLOSED);
}

DWORD WSA::FIO::write(BYTE *b, DWORD len)
{
	if (~this->file)
	{
		return WSA::write(this->file, b, len);
	}
	throw Exception::exception(Exception::exception::EXTERNAL, WSA::FILE_CLOSED);
}

QWORD WSA::FIO::available()
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

void WSA::FIO::seek(QWORD offset) const
{
	WSA::seek(this->file, offset, FILE_BEGIN);
}

void WSA::FIO::close()
{
	if (~this->file)
	{
		WSA::close(this->file);
		this->file = HFILE_ERROR;
	}
}