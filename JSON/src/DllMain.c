#ifndef CMAKE_BUILD

#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1

#pragma section(".CRT$XCA", long, read) // First C++ Initializer
#pragma section(".CRT$XCZ", long, read) // Last C++ Initializer

__declspec(allocate(".CRT$XCA")) void (__cdecl *XCA)(void) = 0;
__declspec(allocate(".CRT$XCZ")) void (__cdecl *XCZ)(void) = 0;

#pragma comment(linker, "/MERGE:.CRT=.rdata")

int _fltused = 1;

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

int _DllMainCRTStartup(void *instance, unsigned int reason, void *reserved)
{
	(void) instance;
	(void) reserved;
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			CRTINIT();
			break;
		}
	}
	return 1;
}

#endif