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
bool Appendix32(QWORD length, Memory::string &block, QWORD &position, void (*save)(QWORD, BYTE, void *))
{
	if (block.length - position < 8)
	{
		Memory::fill(block.address + position, 0, block.length - position);
		position = 0;
		return true;
	}
	Memory::fill(block.address + position, 0, block.length - 8 - position);
	position = block.length - 8;
	save(length, 8, block.address + position);
	position += 8;
	return false;
}
void Transform32(Memory::string &digest, void (*save)(QWORD, BYTE, void *))
{
	DWORD *buf = (DWORD *) digest.address;
	for (QWORD i = 0; i < digest.length >> 2; i++)
	{
		save(buf[i], 4, buf + i);
	}
}
void TransformLE32(Memory::string &digest)
{
	DWORD *buf = (DWORD *) digest.address;
	for (QWORD i = 0; i < digest.length >> 2; i++)
	{
		SaveAsLEndian(buf[i], 4, buf + i);
	}
}
QWORD ROTL(QWORD X, BYTE n) { return (X << n) | (X >> (32 - n)); }
QWORD ROTR(QWORD X, BYTE n) { return (X >> n) | (X << (32 - n)); }
QWORD Ch(QWORD x, QWORD y, QWORD z) { return (x & y) ^ (~x & z); }
QWORD Maj(QWORD x, QWORD y, QWORD z) { return (x & y) ^ (x & z) ^ (y & z); }
QWORD SIGMA0256(QWORD x) { return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22); }
QWORD SIGMA1256(QWORD x) { return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25); }
QWORD Sigma0256(QWORD x) { return ROTR(x, 7) ^ ROTR(x, 18) ^ ROTR(x, 3); }
QWORD Sigma1256(QWORD x) { return ROTR(x, 17) ^ ROTR(x, 19) ^ ROTR(x, 10); }