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

#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
#define FORMAT_MESSAGE_FROM_STRING     0x00000400
#define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF

#define SYMOPT_LOAD_LINES 0x10
#define MAX_SYM_NAME 2000


typedef void *HANDLE, *HINSTANCE, *LPVOID, *PVOID, *HMODULE;
typedef unsigned __int64 ULONG_PTR, SIZE_T;
typedef char CHAR, *va_list;
typedef CHAR *LPSTR;
typedef const char *PCSTR;
typedef unsigned __int64 DWORD64, *PDWORD64, ULONG64;
typedef unsigned long ULONG;
typedef DWORD *PDWORD;
typedef struct
{
	ULONG       SizeOfStruct;
	ULONG       TypeIndex;        // Type Index of symbol
	ULONG64     Reserved[2];
	ULONG       Index;
	ULONG       Size;
	ULONG64     ModBase;          // Base Address of module comtaining this symbol
	ULONG       Flags;
	ULONG64     Value;            // Value of symbol, ValuePresent should be 1
	ULONG64     Address;          // Address of symbol including base address of module
	ULONG       Register;         // register holding value or pointer to value
	ULONG       Scope;            // scope of the symbol
	ULONG       Tag;              // pdb classification
	ULONG       NameLen;          // Actual length of name
	ULONG       MaxNameLen;
	CHAR        Name[1];          // Name of symbol
} SYMBOL_INFO, *PSYMBOL_INFO;

extern Memory::string *ErrorMessage;
extern DWORD ErrorCode;

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
DWORD FormatMessageA(DWORD, const void *, DWORD, DWORD, LPSTR, DWORD, va_list);
HANDLE __stdcall GetCurrentProcess(void);
DWORD __declspec(dllimport) __stdcall SymSetOptions(DWORD);
BOOL __declspec(dllimport) __stdcall SymInitialize(HANDLE, PCSTR, BOOL);
BOOL __declspec(dllimport) __stdcall SymCleanup(HANDLE);
BOOL __declspec(dllimport) __stdcall SymFromAddr(HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO);
DWORD __stdcall K32GetModuleBaseNameA(HANDLE, HMODULE, LPSTR, DWORD);
DWORD __stdcall GetModuleFileNameA(HMODULE, LPSTR, DWORD);
__declspec(dllimport) WORD __stdcall RtlCaptureStackBackTrace(DWORD, DWORD, PVOID *, PDWORD);
inline __cdecl void *operator new(size_t, void *) noexcept;
void backtrace(Memory::exception &);
QWORD strlen(const void *);

#ifdef __cplusplus
}
#endif

extern HANDLE heap;

#endif //MEMDEF_H
