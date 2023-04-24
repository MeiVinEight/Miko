#include "definitions.h"

int __stdcall DllMain(HINSTANCE *, unsigned int reason, void *)
{
	bool result = true;
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			static const WCHAR RNG[] = L"RNG";
			NTSTATUS status = BCryptOpenAlgorithmProvider(&RNG_HANDLE, RNG, nullptr, 0);
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
			NTSTATUS status = BCryptCloseAlgorithmProvider(&RNG_HANDLE, 0);
			result &= !status;
			break;
		}
	}
	return result;
}