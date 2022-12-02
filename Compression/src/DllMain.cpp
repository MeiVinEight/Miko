#include "definitions.h"

int __stdcall DllMain(void *instance, unsigned int reason, void *reserved)
{
	DllMainStaticDictionary(reason);
	return 1;
}