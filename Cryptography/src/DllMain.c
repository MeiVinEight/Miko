#include "Keccak.h"
#include "CNGRNG.h"

#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1

long __stdcall BCryptOpenAlgorithmProvider(void *, const void *, const void *, DWORD);
long __stdcall BCryptCloseAlgorithmProvider(void *, DWORD);

int __stdcall
#ifdef CMAKE_BUILD
DllMain
#else
_DllMainCRTStartup
#endif
(void *instance, unsigned int reason, void *reserved)
{
	int result = 1;
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			long status = BCryptOpenAlgorithmProvider(&RNG_HANDLE, L"RNG", 0, 0);
			result &= !status;
			for (QWORD t = 0; t < 255; t++)
			{
				QWORD R = 0x80;
				for (int i = 0; i < t; i++)
				{
					QWORD R0 = R & 1;
					R ^= (R0 << 8);
					R ^= (R0 << 4);
					R ^= (R0 << 3);
					R ^= (R0 << 2);
					R >>= 1;
				}
				KECCAKRC[t] = R >> 7;
			}
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			long status = BCryptCloseAlgorithmProvider(&RNG_HANDLE, 0);
			result &= !status;
			break;
		}
	}
	return result;
}