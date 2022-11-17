#include "memdef.h"

int __stdcall DllMain(HINSTANCE *instance, unsigned int reason, void *reserved)
{
	heap = 0;
	if (reason == DLL_PROCESS_ATTACH)
	{
		heap = GetProcessHeap();
		if (!heap)
		{
			return 0;
		}
	}
	return 1;
}