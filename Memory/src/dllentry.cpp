#include "definitions.h"

HANDLE heap;

int __stdcall DllMain(HINSTANCE *instance, unsigned int reason, void *reserved)
{
	heap = 0;
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
			return HeapDestroy(heap);
		}
	}
	return 1;
}