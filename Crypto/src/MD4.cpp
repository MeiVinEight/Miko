#include "definitions.h"

/*
 * References: RFC1320 - The MD4 Message-Digest Algorithm
 */

BYTE MD4K[][16] = {
	/* Round 1. */
	 0,  1,  2,  3,  4,  5,  6,  7,
	 8,  9, 10, 11, 12, 13, 14, 15,
	/* Round 2. */
	 0,  4,  8, 12,  1,  5,  9, 13,
	 2,  6, 10, 14,  3,  7, 11, 15,
	/* Round 3. */
	 0,  8,  4, 12,  2, 10,  6, 14,
	 1,  9,  5, 13,  3, 11,  7, 15
};
BYTE MD4S[][4] = {
	{ 3,  7, 11, 19},
	{ 3,  5,  9, 13},
	{ 3,  9, 11, 15}
};
DWORD MD4CONSTANT[] = {0x00000000, 0x5A827999, 0x6ED9EBA1};

constexpr DWORD MD4F(DWORD X, DWORD Y, DWORD Z)
{
	return (X & Y) | (~X & Z);
}
constexpr DWORD MD4G(DWORD X, DWORD Y, DWORD Z)
{
	return (X & Y) ^ (X & Z) ^ (Y & Z);
}
constexpr DWORD MD4H(DWORD X, DWORD Y, DWORD Z)
{
	return X ^ Y ^ Z;
}
void CalculateMD4(const BYTE *X, DWORD ABCD[4])
{
	DWORD AA = ABCD[0];
	DWORD BB = ABCD[1];
	DWORD CC = ABCD[2];
	DWORD DD = ABCD[3];
	DWORD (*(func[3]))(DWORD, DWORD, DWORD) = {MD4F, MD4G, MD4H};
	for (DWORD i = 0; i < 3; i++)
	{
		for (DWORD j = 0; j < 16; j++)
		{
			DWORD idx = (4 - (j % 4)) % 4;
			ABCD[(idx + 0) % 4] += func[i](ABCD[(idx + 1) % 4], ABCD[(idx + 2) % 4], ABCD[(idx + 3) % 4]);
			ABCD[(idx + 0) % 4] += GetAsLEndian(4, X + (MD4K[i][j] << 2));
			ABCD[(idx + 0) % 4] += MD4CONSTANT[i];
			ABCD[(idx + 0) % 4] = ROTL(ABCD[(idx + 0) % 4], MD4S[i][j & 0x3]);
		}
	}
	ABCD[0] += AA;
	ABCD[1] += BB;
	ABCD[2] += CC;
	ABCD[3] += DD;
}

void Crypto::MD4::update(const void *b, QWORD len)
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
			CalculateMD4(this->block, this->ABCD);
		}
	}
}
Memory::string Crypto::MD4::value() const
{
	DWORD abcd[4] = {this->ABCD[0], this->ABCD[1], this->ABCD[2], this->ABCD[3]};
	BYTE blk[64];
	QWORD pos = this->position;
	Memory::copy(blk, this->block, pos);
	blk[pos++] = 0x80;
	if (64 - pos < 8)
	{
		Memory::fill(blk + pos, 0, 64 - pos);
		CalculateMD4(blk, abcd);
		pos = 0;
	}
	Memory::fill(blk + pos, 0, 56 - pos);
	Memory::copy(blk + 56, &this->length, 8);
	CalculateMD4(blk, abcd);
	Memory::string output(16);
	SaveAsLEndian(abcd[0], 4, output.address + 0x0);
	SaveAsLEndian(abcd[1], 4, output.address + 0x4);
	SaveAsLEndian(abcd[2], 4, output.address + 0x8);
	SaveAsLEndian(abcd[3], 4, output.address + 0xC);
	return output;
}