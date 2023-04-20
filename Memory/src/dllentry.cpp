#include "definitions.h"

HANDLE heap;

int __stdcall DllMain(HINSTANCE *, unsigned int reason, void *)
{
	heap = nullptr;
	bool result = true;
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			heap = HeapCreate(0, 0, 0);
			/*
			if (!heap)
			{
				return 0;
			}
			*/
			// symbol system
			SymSetOptions(SYMOPT_LOAD_LINES);
			result &= !!SymInitialize(GetCurrentProcess(), nullptr, 1);

			NTDLL = LoadLibraryA("ntdll.dll");
			result &= NTDLL != nullptr;
			if (NTDLL)
			{
				RtlNtStatusToDosError = (ULONG (*)(NTSTATUS)) GetProcAddress(NTDLL, "RtlNtStatusToDosError");
				result &= RtlNtStatusToDosError != nullptr;
			}
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			delete[] ErrorMessage;
			result &= !!SymCleanup(GetCurrentProcess());
			result &= !!HeapDestroy(heap);
			result &= !!FreeLibrary(NTDLL);
			break;
		}
	}
	return result;
}