#include <wsadef.h>

#pragma comment(lib, "DbgHelp.lib")

#ifdef __cplusplus
extern "C" {
#endif

#define WINAPI __stdcall
#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0
#define SYMOPT_LOAD_LINES 0x10

typedef void *HANDLE;
typedef DWORD *LPDWORD;
typedef void *LPVOID;
typedef const void	*LPCVOID;
typedef unsigned __int64 ULONG_PTR;
typedef void *PVOID;
typedef const char *PCSTR;
typedef void *HINSTANCE;
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

DWORD WINAPI GetLastError(void);
HANDLE WINAPI GetCurrentProcess(void);
DWORD __declspec(dllimport) __stdcall SymSetOptions(DWORD);
BOOL __declspec(dllimport) __stdcall SymInitialize(HANDLE, PCSTR, BOOL);
BOOL __declspec(dllimport) __stdcall SymCleanup(HANDLE);

#ifdef __cplusplus
}
#endif

int WINAPI DllMain(HINSTANCE *instance, unsigned int reason, void *reserved)
{
	(void)&DllMain;
	(void)instance;
	(void)reserved;

	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			SymSetOptions(SYMOPT_LOAD_LINES);
			return SymInitialize(GetCurrentProcess(), 0, 1);
		}
		case DLL_PROCESS_DETACH:
		{
			return SymCleanup(GetCurrentProcess());
		}
	}
	return 1;
}

