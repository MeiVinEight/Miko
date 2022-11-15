#ifndef EXCEDEF_H
#define EXCEDEF_H

#define EXCEPTION_SHARED

#include <exception.h>
#include <memory.h>
#pragma comment(lib, "DbgHelp.lib")

#ifdef __cplusplus
extern "C"
{
#endif

#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0
#define SYMOPT_LOAD_LINES 0x10
#define MAX_SYM_NAME 2000

typedef const char *PCSTR;
typedef void *HINSTANCE;
typedef int BOOL;
typedef char CHAR;
typedef unsigned long ULONG;
typedef unsigned __int64 ULONG64;
typedef unsigned __int64 DWORD64, *PDWORD64;
typedef void *HANDLE;
typedef CHAR *LPSTR;
typedef void *HMODULE;
typedef void *PVOID;
typedef DWORD *PDWORD;
typedef CHAR *PCHAR;
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
typedef struct
{
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE64)
	PVOID    Key;                    // internal
	DWORD    LineNumber;             // line number in file
	PCHAR    FileName;               // full filename
	DWORD64  Address;                // first instruction of line
} IMAGEHLP_LINE64, *PIMAGEHLP_LINE64;


HANDLE __stdcall GetCurrentProcess(void);
DWORD __declspec(dllimport) __stdcall SymSetOptions(DWORD);
BOOL __declspec(dllimport) __stdcall SymInitialize(HANDLE, PCSTR, BOOL);
BOOL __declspec(dllimport) __stdcall SymCleanup(HANDLE);
BOOL __declspec(dllimport) __stdcall SymFromAddr(HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO);
DWORD K32GetModuleBaseNameA(HANDLE, HMODULE, LPSTR, DWORD);
__declspec(dllimport) WORD __stdcall RtlCaptureStackBackTrace(DWORD, DWORD, PVOID *, PDWORD);
BOOL __declspec(dllimport) __stdcall SymGetLineFromAddr64(HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64);
DWORD __stdcall GetLastError(void);
inline void* __cdecl operator new(size_t size, void* where) noexcept
{
	(void) size;
	return where;
}

#ifdef __cplusplus
}
#endif

#endif //EXCEDEF_H
