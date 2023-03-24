#include "definitions.h"

/*
 * References:
 * [1] RFC3174 - US Secure Hash Algorithm 1 (SHA1)
 * [2] FIPS 180-4
 */

DWORD SHA1_K[] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

DWORD F1(DWORD A, DWORD B, DWORD C)
{
	return (A & B) | (~A & C);
}
DWORD F2(DWORD A, DWORD B, DWORD C)
{
	return A ^ B ^ C;
}
DWORD F3(DWORD A, DWORD B, DWORD C)
{
	return (A & B) | (A & C) | (B & C);
}
DWORD F4(DWORD A, DWORD B, DWORD C)
{
	return F2(A, B, C);
}
void CalculateSHA1(DWORD H[5], const BYTE *block)
{
	static DWORD (*(func[4]))(DWORD, DWORD, DWORD) = {F1, F2, F3, F4};
	DWORD W[80];
	DWORD A = H[0];
	DWORD B = H[1];
	DWORD C = H[2];
	DWORD D = H[3];
	DWORD E = H[4];
	for (DWORD t = 0; t < 80; t++)
	{
		W[t] = GetAsBEndian(4, block + 4 * (t & 0xF));
		if (t > 15)
		{
			W[t] = ROTL(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		}
		DWORD TEMP = ROTL(A, 5) + func[t / 20](B, C, D) + E + W[t] + SHA1_K[t / 20];
		E = D;
		D = C;
		C = ROTL(B, 30);
		B = A;
		A = TEMP;
	}
	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;
}

void Crypto::SHA1::update(const void *b, QWORD len)
{
	this->length += len << 3;
	const BYTE *buf = (const BYTE *) b;
	while (len)
	{
		QWORD copy = 64 - this->position;
		copy = copy < len ? copy : len;
		Memory::copy(this->block + this->position, buf, copy);
		buf += copy;
		this->position += copy;
		len -= copy;
		if (this->position >= 64)
		{
			this->position = 0;
			CalculateSHA1(this->H, this->block);
		}
	}
}
Memory::string Crypto::SHA1::value() const
{
	DWORD h[5] = {this->H[0], this->H[1], this->H[2], this->H[3], this->H[4]};
	BYTE blk[64];
	QWORD pos = this->position;
	Memory::copy(blk, this->block, pos);
	blk[pos++] = 0x80;
	if (64 - pos < 8)
	{
		Memory::fill(blk + pos, 0, 64 - pos);
		CalculateSHA1(h, blk);
		pos = 0;
	}
	Memory::fill(blk +pos, 0, 56 - pos);
	SaveAsBEndian((this->length >> 32) & 0xFFFFFFFFU, 4, blk + 56);
	SaveAsBEndian((this->length >>  0) & 0xFFFFFFFFU, 4, blk + 60);
	CalculateSHA1(h, blk);
	Memory::string output(20);
	SaveAsBEndian(h[0], 4, output.address + 0x00);
	SaveAsBEndian(h[1], 4, output.address + 0x04);
	SaveAsBEndian(h[2], 4, output.address + 0x08);
	SaveAsBEndian(h[3], 4, output.address + 0x0C);
	SaveAsBEndian(h[4], 4, output.address + 0x10);
	return output;
}