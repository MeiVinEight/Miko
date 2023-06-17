#include "StaticDictionary.h"

#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1

int __stdcall
#ifdef CMAKE_BUILD
DllMain
#else
_DllMainCRTStartup
#endif
(void *instance, unsigned int reason, void *reserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			DllMainStaticDictionary();
		}
	}
	return 1;
}