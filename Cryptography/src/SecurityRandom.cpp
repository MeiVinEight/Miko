#include "definitions.h"

void Cryptography::SecurityRandom::seed(QWORD)
{
}
QWORD Cryptography::SecurityRandom::random()
{
	const QWORD length = sizeof(QWORD);
	UCHAR buf[length] = {};
	NTSTATUS status = BCryptGenRandom(RNG_HANDLE, buf, length, 0);
	if (status)
		throw Memory::exception(status, Memory::NTSTATUS);
	return *((QWORD *) buf);
}
QWORD Cryptography::SecurityRandom::random(QWORD max)
{
	return this->Cryptography::RNG::random(max);
}