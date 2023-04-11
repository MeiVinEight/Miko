#include "definitions.h"

HANDLE heap;

int __stdcall DllMain(HINSTANCE *, unsigned int reason, void *)
{
	heap = nullptr;
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			heap = HeapCreate(0, 0, 0);
			if (!heap)
			{
				return 0;
			}
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			delete[] ErrorMessage;
			return HeapDestroy(heap);
		}
	}
	return 1;
}