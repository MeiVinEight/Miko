#include <exception.h>
#include <SecurityRandom.h>

#include "CNGRNG.h"

void *RNG_HANDLE = nullptr;

extern "C" long __stdcall BCryptGenRandom(void *, BYTE *, DWORD, DWORD);

void Cryptography::SecurityRandom::seed(QWORD)
{
}
QWORD Cryptography::SecurityRandom::random()
{
	const QWORD length = sizeof(QWORD);
	BYTE buf[8] = {};
	long status = BCryptGenRandom(RNG_HANDLE, buf, length, 0);
	if (status)
		throw Memory::exception(status, Memory::NTSTATUS);
	return *((QWORD *) buf);
}
QWORD Cryptography::SecurityRandom::random(QWORD max)
{
	return this->Cryptography::RNG::random(max);
}