#include "definitions.h"

QWORD GetAsLEndian(BYTE n, const void *p)
{
	const BYTE *addr = (const BYTE *) p;
	DWORD x = 0;
	for (int i = n; i--;)
	{
		x <<= 8;
		x |= addr[i];
	}
	return x;
}
QWORD GetAsBEndian(BYTE n, const void *p)
{
	const BYTE *addr = (const BYTE *) p;
	DWORD x = 0;
	for (int i = 0; i < n; i++)
	{
		x <<= 8;
		x |= addr[i];
	}
	return x;
}
void SaveAsLEndian(QWORD x, BYTE n, void *p)
{
	BYTE *addr = (BYTE *) p;
	for (int i = 0; i < n; i++)
	{
		*addr++ = x & 0xFF;
		x >>= 8;
	}
}
void SaveAsBEndian(QWORD x, BYTE n, void *p)
{
	BYTE *addr = (BYTE *) p;
	addr += n;
	for (int i = 0; i < n; i++)
	{
		*--addr = x & 0xFF;
		x >>= 8;
	}
}
QWORD ROTL(QWORD X, BYTE n)
{
	return (X << n) | (X >> (32 - n));
}
QWORD ROTR(QWORD X, BYTE n)
{
	return (X >> n) | (X << (32 - n));
}