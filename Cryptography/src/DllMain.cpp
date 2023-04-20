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