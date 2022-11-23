#ifndef MEMDEF_H
#define MEMDEF_H

#define MEMORY_SHARED

#include "memory.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1


typedef void *HANDLE, *HINSTANCE, *LPVOID;
typedef unsigned long DWORD;
typedef unsigned __int64 ULONG_PTR, SIZE_T;
typedef int BOOL;

HANDLE __stdcall GetProcessHeap(void);
LPVOID __stdcall HeapAlloc(HANDLE, DWORD, SIZE_T);
LPVOID __stdcall HeapReAlloc(HANDLE, DWORD, LPVOID, SIZE_T);
BOOL __stdcall HeapFree(HANDLE, DWORD, LPVOID);
HANDLE HeapCreate(DWORD, SIZE_T, SIZE_T);
BOOL HeapDestroy(HANDLE);
DWORD GetLastError(void);
void *malloc(SIZE_T);
void *realloc(void *, SIZE_T);
void free(void *);
void *memcpy(void *, const void *, SIZE_T);
void *memset(void *, int, SIZE_T);

#ifdef __cplusplus
}
#endif

extern HANDLE heap;

#endif //MEMDEF_H
