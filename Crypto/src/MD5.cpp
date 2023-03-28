#include "definitions.h"

/*
 * References: RFC1321 - The MD5 Message-Digest Algorithm
 */

BYTE MD5K[][16] = {
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
BYTE MD5S[][4] = {
	{ 7, 12, 17, 22},
	{ 5,  9, 14, 20},
	{ 4, 11, 16, 23},
	{ 6, 10, 15, 21}
};
DWORD MD5T[64] = {
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

constexpr DWORD MD5F(DWORD X, DWORD Y, DWORD Z)
{
	return (X & Y) | (~X & Z);
}
constexpr DWORD MD5G(DWORD X, DWORD Y, DWORD Z)
{
	return (X & Z) | (Y & ~Z);
}
constexpr DWORD MD5H(DWORD X, DWORD Y, DWORD Z)
{
	return X ^ Y ^ Z;
}
constexpr DWORD MD5I(DWORD X, DWORD Y, DWORD Z)
{
	return Y ^ (X | ~Z);
}
void CalculateMD5(BYTE *X, BYTE *digest)
{
	DWORD *ABCD = (DWORD *) digest;
	DWORD AA = ABCD[0];
	DWORD BB = ABCD[1];
	DWORD CC = ABCD[2];
	DWORD DD = ABCD[3];
	DWORD (*(func[4]))(DWORD, DWORD, DWORD) = {MD5F, MD5G, MD5H, MD5I};
	for (DWORD i = 0; i < 4; i++)
	{
		for (DWORD j = 0; j < 16; j++)
		{
			QWORD idx = (4 - (j % 4)) % 4;
			ABCD[(idx + 0) % 4] += func[i](ABCD[(idx + 1) % 4], ABCD[(idx + 2) % 4], ABCD[(idx + 3) % 4]);
			ABCD[(idx + 0) % 4] += GetAsLEndian(4, X + (MD5K[i][j] << 2));
			ABCD[(idx + 0) % 4] += MD5T[i * 16 + j];
			ABCD[(idx + 0) % 4] = ROTL32(ABCD[(idx + 0) % 4], MD5S[i][j & 0x3]);
			ABCD[(idx + 0) % 4] += ABCD[(idx + 1) % 4];
		}
	}
	ABCD[0] += AA;
	ABCD[1] += BB;
	ABCD[2] += CC;
	ABCD[3] += DD;
}

Crypto::MD5::MD5(): MessageDigest(Crypto::MessageDigest::BLOCK_SIZE_32, 16, &CalculateMD5)
{
	DWORD ABCD[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
	Memory::copy(this->digest.address, ABCD, this->digest.length);
}
bool Crypto::MD5::appendix(Memory::string &block, QWORD &position)
{
	return Appendix32(this->length, block, position, &SaveAsLEndian);
}
void Crypto::MD5::transform(Memory::string &digest)
{
	Transform32(digest, &SaveAsLEndian);
}