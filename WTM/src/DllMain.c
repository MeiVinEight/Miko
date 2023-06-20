#include "definitions.h"

#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1

void *__stdcall GetModuleHandleA(const void *);

int __stdcall
#ifdef CMAKE_BUILD
DllMain
#else
_DllMainCRTStartup
#endif
(void *instance, unsigned int reason, void *reserved)
{
	int ret = 1;
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			NTDLL = GetModuleHandleA("ntdll.dll");
			ret &= NTDLL != 0;
			if (NTDLL)
			{
				ZwSuspendProcess = (long (__stdcall *)(void *)) GetProcAddress(NTDLL, "ZwSuspendProcess");
				ret &= ZwSuspendProcess != 0;
				ZwResumeProcess = (long (__stdcall *)(void *)) GetProcAddress(NTDLL, "ZwResumeProcess");
				ret &= ZwResumeProcess != 0;
				NtQueryInformationProcess
				= (long (*)(void *, DWORD, void *, DWORD, DWORD *)) GetProcAddress(NTDLL, "NtQueryInformationProcess");
				ret &= NtQueryInformationProcess != 0;
			}
			break;
		}
	}
	return ret;
}