#include "definitions.h"

DWORD GetAsLEndian(const void *p)
{
	const BYTE *addr = (const BYTE *) p;
	DWORD x = 0;
	for (int i = 4; i--;)
	{
		x <<= 8;
		x |= addr[i];
	}
	return x;
}
DWORD GetAsBEndian(const void *p)
{
	const BYTE *addr = (const BYTE *) p;
	DWORD x = 0;
	for (int i = 0; i < 4; i++)
	{
		x <<= 8;
		x |= addr[i];
	}
	return x;
}
void SaveAsLEndian(DWORD x, void *p)
{
	BYTE *addr = (BYTE *) p;
	for (int i = 0; i < 4; i++)
	{
		*addr++ = x & 0xFF;
		x >>= 8;
	}
}
void SaveAsBEndian(DWORD x, void *p)
{
	BYTE *addr = (BYTE *) p;
	addr += 4;
	for (int i = 0; i < 4; i++)
	{
		*--addr = x & 0xFF;
		x >>= 8;
	}
}
DWORD CircularLSH(DWORD X, BYTE n)
{
	return (X << n) | (X >> (32 - n));
}