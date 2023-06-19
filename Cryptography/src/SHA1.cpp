#include <endian.h>
#include <SHA1.h>

#include "rotate.h"
#include "CommonMessageDigest.h"

/*
 * References:
 * [1] RFC3174 - US Secure Hash Algorithm 1 (SHA1)
 * [2] FIPS 180-4
 */

DWORD SHA1K[] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

DWORD SHA1F1(DWORD A, DWORD B, DWORD C)
{
	return (A & B) | (~A & C);
}
DWORD SHA1F2(DWORD A, DWORD B, DWORD C)
{
	return A ^ B ^ C;
}
DWORD SHA1F3(DWORD A, DWORD B, DWORD C)
{
	return (A & B) | (A & C) | (B & C);
}
void CalculateSHA1(BYTE *block, BYTE *digest)
{
	DWORD *H = (DWORD *) digest;
	static DWORD (*(func[4]))(DWORD, DWORD, DWORD) = {SHA1F1, SHA1F2, SHA1F3, SHA1F2};
	DWORD W[80];
	DWORD A = H[0];
	DWORD B = H[1];
	DWORD C = H[2];
	DWORD D = H[3];
	DWORD E = H[4];
	for (DWORD t = 0; t < 80; t++)
	{
		W[t] = Memory::BE::get(block + 4 * (t & 0xF), 4);
		if (t > 15)
		{
			W[t] = ROTL32(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		}
		DWORD TEMP = ROTL32(A, 5) + func[t / 20](B, C, D) + E + W[t] + SHA1K[t / 20];
		E = D;
		D = C;
		C = ROTL32(B, 30);
		B = A;
		A = TEMP;
	}
	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;
}

Cryptography::SHA1::SHA1(): MessageDigest(Cryptography::MessageDigest::BLOCK_SIZE_32, 20, &CalculateSHA1)
{
	DWORD H[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
	Memory::copy(this->digest.address, H, this->digest.length);
}
bool Cryptography::SHA1::appendix(Memory::string &block, QWORD &position)
{
	return Appendix32(this->length, block, position, &Memory::BE::set);
}
void Cryptography::SHA1::transform(Memory::string &digest)
{
	Transform32(digest, &Memory::BE::set);
}