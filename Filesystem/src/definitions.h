#ifndef FSDEF_H
#define FSDEF_H

#define FS_SHARED

#include <filesystem.h>
#include <exception.h>


#ifdef __cplusplus
extern "C"
{
#endif

#define WINAPI						__stdcall
#define NULL						0
#define INVALID_HANDLE_VALUE		((HANDLE)(LONG_PTR)-1)
#define INVALID_FILE_ATTRIBUTES		(-1)
#define MAX_PATH					260

#define GENERIC_ALL					(0x10000000L)

#define FILE_SHARE_READ				0x00000001
#define FILE_SHARE_WRITE			0x00000002
#define FILE_SHARE_DELETE			0x00000004

#define CREATE_NEW					1
#define FILE_ATTRIBUTE_DIRECTORY	0x00000010
#define FILE_ATTRIBUTE_NORMAL		0x00000080

#define OFS_MAXPATHNAME				128

#define FILE_BEGIN					0
#define FILE_CURRENT				1

#define FILE_TYPE_DISK				0x0001

#define ERROR_FILE_NOT_FOUND		2L
#define ERROR_PATH_NOT_FOUND		3L
#define ERROR_FILE_EXISTS			80L

#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)

typedef int			HFILE;
typedef char		*LPSTR;
typedef const char	*LPCSTR;
typedef void		*HANDLE, *PVOID, *LPVOID;
typedef const void	*LPCVOID;
typedef DWORD		*LPDWORD;
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
} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;
typedef enum
{
	GetFileExInfoStandard,
	GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;

HANDLE WINAPI CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
BOOL WINAPI CloseHandle(HANDLE);
HFILE WINAPI OpenFile(LPCSTR, LPOFSTRUCT, UINT);
DWORD WINAPI GetLastError(void);
BOOL WINAPI ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
BOOL WINAPI WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
BOOL WINAPI SetFilePointerEx(HANDLE, LARGE_INTEGER, PLARGE_INTEGER, DWORD);
DWORD WINAPI GetFileType(HANDLE);
BOOL WINAPI SetFileAttributesA(LPCSTR, DWORD);
DWORD WINAPI GetFileAttributesA(LPCSTR);
BOOL WINAPI RemoveDirectoryA(LPCSTR);
BOOL WINAPI DeleteFileA(LPCSTR);
DWORD WINAPI GetFullPathNameA(LPCSTR, DWORD, LPSTR, LPSTR *);
BOOL WINAPI GetFileAttributesExA(LPCSTR, GET_FILEEX_INFO_LEVELS, LPVOID);
BOOL WINAPI CreateDirectoryA(LPCSTR, LPSECURITY_ATTRIBUTES);
BOOL WINAPI FlushFileBuffers(HANDLE);
HANDLE WINAPI GetStdHandle(DWORD);
BOOL WINAPI GetFileSizeEx(HANDLE, PLARGE_INTEGER);

#ifdef __cplusplus
}
#endif

QWORD strlen(const void *);
Memory::string cstring(const String::string &);

#endif //FSDEF_H
