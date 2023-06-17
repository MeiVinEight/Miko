#include "definitions.h"
#include "crt.h"
#include "C_specific_handler.h"
#include "ErrorMessage.h"


#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1
#define SYMOPT_LOAD_LINES 0x10

extern "C"
{

#ifndef CMAKE_BUILD

#include "ThreadLocalStorage.h"

#pragma section(".CRT$XCA", long, read) // First C++ Initializer
#pragma section(".CRT$XCZ", long, read) // Last C++ Initializer

__declspec(allocate(".CRT$XCA")) void (__cdecl *XCA)(void) = nullptr;
__declspec(allocate(".CRT$XCZ")) void (__cdecl *XCZ)(void) = nullptr;

#pragma comment(linker, "/MERGE:.CRT=.rdata")

void CRTINIT()
{
	void (__cdecl **xca)(void) = &XCA;
	while (xca < &XCZ)
	{
		if (*xca)
		{
			(*xca)();
		}
		xca++;
	}
}
int (*(scrt[][2]))() = {
	{&InitializeTLS, &UninitializeTLS},
	{nullptr, nullptr}
};

int scrt_init()
{
	int result = 1;
	unsigned long long i = 0;
	while (result && (scrt[i][0] || scrt[i][1]))
	{
		if (scrt[i][0])
		{
			result &= scrt[i][0]();
		}
		i++;
	}
	return result;
}
int scrt_uninit()
{
	int result = 1;
	unsigned long long i = 0;
	while (scrt[i][0] || scrt[i][1]) i++;
	while (result && i--)
	{
		if (scrt[i][1])
		{
			result &= scrt[i][1]();
		}
	}
	return result;
}

void onexit(void);

#else

void CRTINIT()
{
}
int scrt_init()
{
	return 1;
}
int scrt_uninit()
{
	return 1;
}
void onexit(void)
{
}

#endif

HANDLE __stdcall GetProcessHeap(void);
DWORD __declspec(dllimport) __stdcall SymSetOptions(DWORD);
BOOL __declspec(dllimport) __stdcall SymInitialize(HANDLE, PCSTR, BOOL);
BOOL __declspec(dllimport) __stdcall SymCleanup(HANDLE);
FARPROC __stdcall GetProcAddress(void *, const void *);
HMODULE __stdcall GetModuleHandleA(const void *);
void *__stdcall GetCurrentProcess(void);

HMODULE NTDLL = nullptr;


int __stdcall
#ifdef CMAKE_BUILD
DllMain
#else
_DllMainCRTStartup
#endif
(HINSTANCE, unsigned int reason, void *)
{
	int result = true;
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			heap = GetProcessHeap();
			result &= heap != nullptr;

			// symbol system
			SymSetOptions(SYMOPT_LOAD_LINES);
			result &= !!SymInitialize(GetCurrentProcess(), nullptr, 1);

			NTDLL = GetModuleHandleA("ntdll.dll");
			result &= NTDLL != nullptr;
			if (NTDLL)
			{
				RtlNtStatusToDosError = (ULONG (*)(NTSTATUS)) GetProcAddress(NTDLL, "RtlNtStatusToDosError");
				result &= RtlNtStatusToDosError != nullptr;
				NTDLL___C_specific_handler = (C_SPECIFIC_HANDLER) GetProcAddress(NTDLL, "__C_specific_handler");
				result &= NTDLL___C_specific_handler != nullptr;
			}

			CRTINIT();
			result &= scrt_init();
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			onexit();
			delete[] ErrorMessage;
			scrt_uninit();
			SymCleanup(GetCurrentProcess());
			break;
		}
	}
	return result;
}

}