#ifndef CMAKE_BUILD


#pragma section(".CRT$XCA", long, read)
#pragma section(".CRT$XCZ", long, read)

__declspec(allocate(".CRT$XCA")) void (*XCA)() = 0;
__declspec(allocate(".CRT$XCZ")) void (*XCZ)() = 0;

#pragma comment(linker, "/MERGE:.CRT=.rdata")

int __stdcall _DllMainCRTStartup(void *ists, int reason, void *resv)
{
	if (reason)
	{
		void (**xca)() = &XCA;
		while (xca < &XCZ)
		{
			if (*xca)
			{
				(*xca)();
			}
			xca++;
		}
	}
	return 1;
}


#endif