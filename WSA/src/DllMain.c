#include "WinType.h"

#define WSADESCRIPTION_LEN  256
#define WSASYS_STATUS_LEN   128
#define DLL_PROCESS_DETACH  0
#define DLL_PROCESS_ATTACH  1
#define SOCKET_ERROR        (-1)

typedef struct
{
	WORD					wVersion;
	WORD					wHighVersion;
	unsigned short			iMaxSockets;
	unsigned short			iMaxUdpDg;
	char *					lpVendorInfo;
	char					szDescription[WSADESCRIPTION_LEN+1];
	char					szSystemStatus[WSASYS_STATUS_LEN+1];
} WSADATA, *LPWSADATA;

int __stdcall WSAStartup(WORD, void *);
int __stdcall WSACleanup(void);

#ifdef CMAKE_BUILD

void CRTINIT()
{
}

#else

#pragma section(".CRT$XCA", long, read) // First C++ Initializer
#pragma section(".CRT$XCZ", long, read) // Last C++ Initializer

__declspec(allocate(".CRT$XCA")) void (__cdecl *XCA)(void) = 0;
__declspec(allocate(".CRT$XCZ")) void (__cdecl *XCZ)(void) = 0;

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

#endif

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
			WSADATA data;
			int result = WSAStartup(0x0202, &data);
			CRTINIT();
			return result;
		}
		case DLL_PROCESS_DETACH:
		{
			return WSACleanup() != SOCKET_ERROR;
		}
	}
	return 1;
}