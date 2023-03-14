#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define STREAMING_SHARED
#include <streaming.h>

#define WINAPI						__stdcall

#define FILE_TYPE_DISK				0x0001
#define FILE_TYPE_CHAR				0x0002
#define FILE_TYPE_PIPE				0x0003

#define FILE_BEGIN					0
#define FILE_CURRENT				1

typedef long			LONG;
typedef void			*HANDLE;
typedef __int64 		LONGLONG;
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

#ifdef __cplusplus
extern "C"
{
#endif

DWORD WINAPI GetFileType(HANDLE);
BOOL WINAPI SetFilePointerEx(HANDLE, LARGE_INTEGER, PLARGE_INTEGER, DWORD);
BOOL WINAPI GetFileSizeEx(HANDLE, PLARGE_INTEGER);

#ifdef __cplusplus
}
#endif

#endif //DEFINITIONS_H
