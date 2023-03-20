#include "definitions.h"

DWORD GetAsLittleEndian(const BYTE *addr)
{
	DWORD x = 0;
	for (int i = 4; i--;)
	{
		x <<= 8;
		x |= addr[i];
	}
	return x;
}
DWORD CircularLSH(DWORD X, BYTE n)
{
	return (X << n) | (X >> (32 - n));
}