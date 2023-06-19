#ifndef MEMDEF_H
#define MEMDEF_H

#include <WinType.h>


typedef void *HANDLE, *HINSTANCE, *LPVOID, *PVOID, *HMODULE;
typedef __int64 INT_PTR;
typedef unsigned __int64 ULONG_PTR, SIZE_T, ULONGLONG, DWORD64, *PDWORD64, ULONG64;
typedef char CHAR, *va_list;
typedef char *LPSTR;
typedef const char *PCSTR;
typedef const char *LPCSTR;
typedef long LONG, NTSTATUS;
typedef unsigned long ULONG;
typedef DWORD *PDWORD;
typedef const void *LPCVOID;
typedef INT_PTR (__stdcall *FARPROC)();


#endif //MEMDEF_H
