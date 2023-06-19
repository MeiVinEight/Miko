#include "definitions.h"
#pragma comment(lib, "bcrypt.lib")

BCRYPT_ALG_HANDLE RNG_HANDLE = nullptr;
BYTE KECCAKRC[255];

QWORD MASK(BYTE x)
{
	x &= 0x3F;
	x = 64 - x;
	return (0xFFFFFFFFFFFFFFFF << x) >> x;
}
QWORD GetAsLEndian(BYTE n, const void *p)
{
	const BYTE *addr = (const BYTE *) p;
	QWORD x = 0;
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
	QWORD x = 0;
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
bool Appendix32(QWORD length, Memory::string &block, QWORD &position, void (*save)(QWORD, void *, BYTE))
{
	if (block.length - position < 8)
	{
		Memory::fill(block.address + position, 0, block.length - position);
		position = 0;
		return true;
	}
	Memory::fill(block.address + position, 0, block.length - 8 - position);
	position = block.length - 8;
	save(length, block.address + position, 8);
	position += 8;
	return false;
}
bool Appendix64(QWORD length, Memory::string &block, QWORD &position, void (*save)(QWORD, void *, BYTE))
{
	if (block.length - position < 16)
	{
		Memory::fill(block.address + position, 0, block.length - position);
		position = 0;
		return true;
	}
	Memory::fill(block.address + position, 0, block.length - 16 - position);
	position = block.length - 16;
	save(0, block.address + position, 8);
	position += 8;
	save(length, block.address + position, 8);
	position += 16;
	return false;
}
void Transform32(Memory::string &digest, void (*save)(QWORD, void *, BYTE))
{
	DWORD *buf = (DWORD *) digest.address;
	for (QWORD i = 0; i < digest.length >> 2; i++)
	{
		save(buf[i], buf + i, 4);
	}
}
void Transform64(Memory::string &digest, void (*save)(QWORD, void *, BYTE))
{
	QWORD *buf = (QWORD *) digest.address;
	for (QWORD i = 0; i < digest.length >> 3; i++)
	{
		save(buf[i], buf + i, 8);
	}
}
QWORD ROTL(QWORD x, BYTE n, BYTE w) { return ((x << (n % w)) | (x >> (w - (n % w)))) & MASK(w); }
QWORD ROTR(QWORD x, BYTE n, BYTE w) { return ((x >> (n % w)) | (x << (w - (n % w)))) & MASK(w); }
QWORD ROTL32(QWORD X, BYTE n) { return ROTL(X, n, 32); }
QWORD ROTR32(QWORD X, BYTE n) { return ROTR(X, n, 32); }
QWORD ROTL64(QWORD X, BYTE n) { return ROTL(X, n, 64); }
QWORD ROTR64(QWORD X, BYTE n) { return ROTR(X, n, 64); }
QWORD Ch(QWORD x, QWORD y, QWORD z) { return (x & y) ^ (~x & z); }
QWORD Maj(QWORD x, QWORD y, QWORD z) { return (x & y) ^ (x & z) ^ (y & z); }
QWORD SIGMA0256(QWORD x) { return ROTR32(x, 2) ^ ROTR32(x, 13) ^ ROTR32(x, 22); }
QWORD SIGMA1256(QWORD x) { return ROTR32(x, 6) ^ ROTR32(x, 11) ^ ROTR32(x, 25); }
QWORD Sigma0256(QWORD x) { return ROTR32(x, 7) ^ ROTR32(x, 18) ^ (x >> 3); }
QWORD Sigma1256(QWORD x) { return ROTR32(x, 17) ^ ROTR32(x, 19) ^ (x >> 10); }
QWORD SIGMA0512(QWORD x) { return ROTR64(x, 28) ^ ROTR64(x, 34) ^ ROTR64(x, 39); }
QWORD SIGMA1512(QWORD x) { return ROTR64(x, 14) ^ ROTR64(x, 18) ^ ROTR64(x, 41); }
QWORD Sigma0512(QWORD x) { return ROTR64(x,  1) ^ ROTR64(x, 8) ^ (x >> 7); }
QWORD Sigma1512(QWORD x) { return ROTR64(x, 19) ^ ROTR64(x, 61) ^ (x >> 6); }