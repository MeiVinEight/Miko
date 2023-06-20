#ifndef WTM_DEFINITIONS_H
#define WTM_DEFINITIONS_H

#include <WinType.h>

#define DUPLICATE_SAME_ACCESS       0x00000002

#define WAIT_ABANDONED (0x00000080UL)
#define WAIT_OBJECT_0  (0x00000000UL)
#define WAIT_TIMEOUT   (0x00000102UL)
#define WAIT_FAILED    (0xFFFFFFFFUL)

#ifdef __cplusplus
extern "C"
{
#endif


extern void *NTDLL;
extern long (__stdcall *ZwSuspendProcess)(void *);
extern long (__stdcall *ZwResumeProcess)(void *);
extern long (__stdcall *NtQueryInformationProcess)(void *, DWORD, void *, DWORD, DWORD *);

void *__stdcall GetCurrentProcess(void);
void *__stdcall GetCurrentThread(void);
BOOL __stdcall DuplicateHandle(void *, void *, void *, void **, DWORD, BOOL, DWORD);
BOOL __stdcall CloseHandle(void *);
DWORD __stdcall WaitForSingleObject(void *, DWORD);
DWORD __stdcall SuspendThread(void *);
DWORD __stdcall ResumeThread(void *);
void __stdcall Sleep(DWORD);
void * __stdcall CreateThread(void *, QWORD, DWORD (*)(void *), void *, DWORD, void *);
DWORD GetThreadId(void *);
BOOL __stdcall GetExitCodeThread(void *, DWORD *);
long __stdcall GetThreadDescription(void *, wchar_t * *);
long __stdcall SetThreadDescription(void *, wchar_t *);
int __stdcall WideCharToMultiByte(unsigned int, DWORD, const wchar_t *, int, char *, int, const char *, BOOL *);
int __stdcall MultiByteToWideChar(unsigned int, DWORD, const char *, int, wchar_t *, int);
void *__stdcall LocalFree(void *);
void *__stdcall LoadLibraryA(const char *);
QWORD (__stdcall *__stdcall GetProcAddress(void *, const char *))();
BOOL __stdcall FreeLibrary(void *);
DWORD __stdcall GetProcessId(void *);
void *__stdcall OpenProcess(DWORD, BOOL, DWORD);
BOOL __stdcall OpenProcessToken(void *, DWORD, void *);
BOOL __stdcall LookupPrivilegeValueA(const char *, const char *, void *);
BOOL __stdcall AdjustTokenPrivileges(void *, BOOL, void *, DWORD, void *, DWORD *);
DWORD __stdcall QueryFullProcessImageNameA(void *, DWORD, char *, DWORD *);
BOOL TerminateProcess(void *, unsigned int);
void *__stdcall CreateToolhelp32Snapshot(DWORD, DWORD);

#ifdef __cplusplus
}
#endif

#endif //WTM_DEFINITIONS_H
