#include "wsadef.h"

int __stdcall DllMain(HINSTANCE *instance, unsigned int reason, void *reserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			WSADATA data;
			return !WSAStartup(0x0202, &data);
		}
		case DLL_PROCESS_DETACH:
		{
			return WSACleanup() != SOCKET_ERROR;
		}
	}
	return 1;
}