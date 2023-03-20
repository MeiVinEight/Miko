#include "definitions.h"

BYTE K[][16] = {
	/* Round 1. */
	 0,  1,  2,  3,  4,  5,  6,  7,
	 8,  9, 10, 11, 12, 13, 14, 15,
	/* Round 2. */
	 1,  6, 11,  0,  5, 10, 15,  4,
	 9, 14,  3,  8, 13,  2,  7, 12,
	/* Round 3. */
	 5,  8, 11, 14,  1,  4,  7, 10,
	13,  0,  3,  6,  9, 12, 15,  2,
	/* Round 4. */
	 0,  7, 14,  5, 12,  3, 10,  1,
	 8, 15,  6, 13,  4, 11,  2,  9
};
BYTE S[][4] = {
	{ 7, 12, 17, 22},
	{ 5,  9, 14, 20},
	{ 4, 11, 16, 23},
	{ 6, 10, 15, 21}
};
DWORD T[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

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
constexpr DWORD F(DWORD X, DWORD Y, DWORD Z)
{
	return (X & Y) | (~X & Z);
}
constexpr DWORD G(DWORD X, DWORD Y, DWORD Z)
{
	return (X & Z) | (Y & ~Z);
}
constexpr DWORD H(DWORD X, DWORD Y, DWORD Z)
{
	return X ^ Y ^ Z;
}
constexpr DWORD I(DWORD X, DWORD Y, DWORD Z)
{
	return Y ^ (X | ~Z);
}
constexpr DWORD SS(DWORD X, BYTE n)
{
	return (X << n) | (X >> (32 - n));
}
void Calculate(const BYTE *X, DWORD ABCD[4])
{
	DWORD AA = ABCD[0];
	DWORD BB = ABCD[1];
	DWORD CC = ABCD[2];
	DWORD DD = ABCD[3];
	DWORD (*(func[4]))(DWORD, DWORD, DWORD) = {F, G, H, I};
	for (DWORD i = 0; i < 4; i++)
	{
		for (DWORD j = 0; j < 16; j++)
		{
			QWORD idx = (4 - (j % 4)) % 4;
			ABCD[(idx + 0) % 4] += func[i](ABCD[(idx + 1) % 4], ABCD[(idx + 2) % 4], ABCD[(idx + 3) % 4]);
			ABCD[(idx + 0) % 4] += GetAsLittleEndian(X + (K[i][j] << 2));
			ABCD[(idx + 0) % 4] += T[i * 16 + j];
			ABCD[(idx + 0) % 4] = SS(ABCD[(idx + 0) % 4], S[i][j & 0x3]);
			ABCD[(idx + 0) % 4] += ABCD[(idx + 1) % 4];
		}
	}
	ABCD[0] += AA;
	ABCD[1] += BB;
	ABCD[2] += CC;
	ABCD[3] += DD;
}

void Crypto::MD5::update(const void *b, QWORD len)
{
	this->length += len << 3;
	const char *buf = (const char *) b;
	while (len)
	{
		QWORD copy = 64 - this->position;
		copy = len < copy ? len : copy;
		Memory::copy(this->block + this->position, buf, copy);
		buf += copy;
		this->position += copy;
		len -= copy;
		if (this->position >= 64)
		{
			this->position = 0;
			Calculate(this->block, this->ABCD);
		}
	}
}
Memory::string Crypto::MD5::value() const
{
	DWORD abcd[4] = {this->ABCD[0], this->ABCD[1], this->ABCD[2], this->ABCD[3]};
	BYTE blk[64];
	QWORD pos = this->position;
	Memory::copy(blk, this->block, pos);
	blk[pos++] = 0x80;
	if (64 - pos < 8)
	{
		Memory::fill(blk + pos, 0, 64 - pos);
		Calculate(blk, abcd);
		pos = 0;
	}
	Memory::fill(blk + pos, 0, 56 - pos);
	Memory::copy(blk + 56, &this->length, 8);
	Calculate(blk, abcd);
	Memory::string output(16);
	abcd[0] = GetAsLittleEndian((BYTE *) (abcd + 0));
	abcd[1] = GetAsLittleEndian((BYTE *) (abcd + 1));
	abcd[2] = GetAsLittleEndian((BYTE *) (abcd + 2));
	abcd[3] = GetAsLittleEndian((BYTE *) (abcd + 3));
	Memory::copy(output.address, abcd, 16);
	return output;
}