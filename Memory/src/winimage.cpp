#include <winimage.h>
#include <WinType.h>

extern "C"
{

BOOL __stdcall GetModuleHandleExA(DWORD, const void *, void **);

}

void *Memory::image(void *addr)
{
	void *base = nullptr;
	GetModuleHandleExA(6, addr, &base);
	return base;
}