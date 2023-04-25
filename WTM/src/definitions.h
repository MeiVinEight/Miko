#ifndef WTM_DEFINITIONS_H
#define WTM_DEFINITIONS_H

#define WTM_SHARED
#include <WTM.h>

#define DUPLICATE_SAME_ACCESS       0x00000002

#define WAIT_ABANDONED (0x00000080UL)
#define WAIT_OBJECT_0  (0x00000000UL)
#define WAIT_TIMEOUT   (0x00000102UL)
#define WAIT_FAILED    (0xFFFFFFFFUL)

typedef char              CHAR;
typedef wchar_t           WCHAR;
typedef unsigned int      UINT;
typedef int               BOOL;
typedef long              HRESULT;
typedef unsigned __int64  SIZE_T;
typedef void             *HANDLE;
typedef CHAR             *LPSTR;
typedef const CHAR       *LPCCH;
typedef WCHAR            *PWSTR, *LPWSTR;
typedef const WCHAR      *PCWSTR, *LPCWCH;
typedef BOOL             *LPBOOL;
typedef HANDLE           *LPHANDLE, HLOCAL;
typedef DWORD (__stdcall *PTHREAD_START_ROUTINE)(void *);

#ifdef __cplusplus
extern "C"
{
#endif

DWORD __stdcall GetLastError(void);
HANDLE __stdcall GetCurrentProcess(void);
HANDLE __stdcall GetCurrentThread(void);
BOOL __stdcall DuplicateHandle(HANDLE, HANDLE, HANDLE, LPHANDLE, DWORD, BOOL, DWORD);
BOOL __stdcall CloseHandle(HANDLE);
DWORD __stdcall WaitForSingleObject(HANDLE, DWORD);
DWORD __stdcall SuspendThread(HANDLE);
DWORD __stdcall ResumeThread(HANDLE);
void __stdcall Sleep(DWORD);
HANDLE __stdcall CreateThread(void *, SIZE_T, PTHREAD_START_ROUTINE, void *, DWORD, void *);
DWORD GetThreadId(HANDLE);
BOOL __stdcall GetExitCodeThread(HANDLE, DWORD *);
HRESULT __stdcall GetThreadDescription(HANDLE, PWSTR *);
HRESULT __stdcall SetThreadDescription(HANDLE, PCWSTR);
int __stdcall WideCharToMultiByte(UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL);
int __stdcall MultiByteToWideChar(UINT, DWORD, LPCCH, int, LPWSTR, int);
HLOCAL __stdcall LocalFree(HLOCAL);
QWORD StringLengthW(const void *);

#ifdef __cplusplus
}
#endif

#endif //WTM_DEFINITIONS_H
