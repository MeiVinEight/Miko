#include "definitions.h"
#pragma comment(lib, "bcrypt.lib")

BCRYPT_ALG_HANDLE RNG_HANDLE = nullptr;
BYTE KECCAKRC[255];

QWORD ROTMASK[65] = {
	0x0000000000000000,
	0x0000000000000001, 0x0000000000000003, 0x0000000000000007, 0x000000000000000F,
	0x000000000000001F, 0x000000000000003F, 0x000000000000007F,	0x00000000000000FF,
	0x00000000000001FF,	0x00000000000003FF,	0x00000000000007FF,	0x0000000000000FFF,
	0x0000000000001FFF,	0x0000000000003FFF,	0x0000000000007FFF,	0x000000000000FFFF,
	0x000000000001FFFF,	0x000000000003FFFF,	0x000000000007FFFF,	0x00000000000FFFFF,
	0x00000000001FFFFF,	0x00000000003FFFFF,	0x00000000007FFFFF,	0x0000000000FFFFFF,
	0x0000000001FFFFFF,	0x0000000003FFFFFF,	0x0000000007FFFFFF,	0x000000000FFFFFFF,
	0x000000001FFFFFFF,	0x000000003FFFFFFF,	0x000000007FFFFFFF,	0x00000000FFFFFFFF,
	0x00000001FFFFFFFF,	0x00000003FFFFFFFF,	0x00000007FFFFFFFF,	0x0000000FFFFFFFFF,
	0x0000001FFFFFFFFF,	0x0000003FFFFFFFFF,	0x0000007FFFFFFFFF,	0x000000FFFFFFFFFF,
	0x000001FFFFFFFFFF,	0x000003FFFFFFFFFF,	0x000007FFFFFFFFFF,	0x00000FFFFFFFFFFF,
	0x00001FFFFFFFFFFF,	0x00003FFFFFFFFFFF,	0x00007FFFFFFFFFFF,	0x0000FFFFFFFFFFFF,
	0x0001FFFFFFFFFFFF,	0x0003FFFFFFFFFFFF,	0x0007FFFFFFFFFFFF,	0x000FFFFFFFFFFFFF,
	0x001FFFFFFFFFFFFF,	0x003FFFFFFFFFFFFF,	0x007FFFFFFFFFFFFF,	0x00FFFFFFFFFFFFFF,
	0x01FFFFFFFFFFFFFF,	0x03FFFFFFFFFFFFFF,	0x07FFFFFFFFFFFFFF,	0x0FFFFFFFFFFFFFFF,
	0x1FFFFFFFFFFFFFFF,	0x3FFFFFFFFFFFFFFF,	0x7FFFFFFFFFFFFFFF,	0xFFFFFFFFFFFFFFFF,
};

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
bool Appendix64(QWORD length, Memory::string &block, QWORD &position, void (*save)(QWORD, BYTE, void *))
{
	if (block.length - position < 16)
	{
		Memory::fill(block.address + position, 0, block.length - position);
		position = 0;
		return true;
	}
	Memory::fill(block.address + position, 0, block.length - 16 - position);
	position = block.length - 16;
	save(0, 8, block.address + position);
	position += 8;
	save(length, 8, block.address + position);
	position += 16;
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
void Transform64(Memory::string &digest, void (*save)(QWORD, BYTE, void *))
{
	QWORD *buf = (QWORD *) digest.address;
	for (QWORD i = 0; i < digest.length >> 3; i++)
	{
		save(buf[i], 8, buf + i);
	}
}
QWORD ROTL(QWORD x, BYTE n, BYTE w) { return ((x << (n % w)) | (x >> (w - (n % w)))) & ROTMASK[w]; }
QWORD ROTR(QWORD x, BYTE n, BYTE w) { return ((x >> (n % w)) | (x << (w - (n % w)))) & ROTMASK[w]; }
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