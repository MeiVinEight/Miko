#include "C_specific_handler.h"

C_SPECIFIC_HANDLER NTDLL___C_specific_handler = 0;

#ifndef CMAKE_BUILD


__declspec(dllexport) EXCEPTION_DISPOSITION __stdcall __C_specific_handler(void * v1, void *v2, void *v3, void *v4)
{
	return NTDLL___C_specific_handler(v1, v2, v3, v4);
}


#endif