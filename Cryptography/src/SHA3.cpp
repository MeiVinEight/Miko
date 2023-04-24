#include "SHA3.h"

const QWORD SHA3FUNCTIONS[4] = {224, 256, 384, 512};

void KeccakP(BYTE l, BYTE nr, BYTE *S, BYTE *T)
{
	SPONGE sponge;
	sponge.L = l;
	BYTE w = 1 << l;
	NibbleReader nibble;

	// STEP1: Convert S into state array, A, as descrined in FIPS 202 Sec. 3.1.2.
	nibble.stream = S;
	nibble.position = 0;
	for (QWORD y = 0; y < 5; y++)
	{
		for (QWORD x = 0; x < 5; x++)
		{
			sponge.A[x][y] = nibble.read(w);
		}
	}

	// STEP2: For ir from 12 + 2l - nr to 12 + 2l - 1, let A = Rnd(A, ir).
	QWORD t = 12 + 2*l;
	QWORD s = t - nr;
	for (QWORD ir = s; ir < t; ir++)
	{
		// Rnd(A, ir) = ι(χ(π(ρ(θ(A)))), ir).
		sponge.theta();
		sponge.rho();
		sponge.pi();
		sponge.chi();
		sponge.iota(ir);
	}

	// STEP3: Convert A into a string S′ of length b, as described in FIPS 202 Sec. 3.1.3.
	nibble.stream = T;
	nibble.position = 0;
	for (QWORD y = 0; y < 5; y++)
	{
		for (QWORD x = 0; x < 5; x++)
		{
			nibble.write(sponge.A[x][y], w);
		}
	}

	// STEP4: Return S′.
}
void KeccakF(BYTE l, BYTE *S, BYTE *T)
{
	KeccakP(l, 12 + 2*l, S, T);
	// return KeccakP1(S);
}
void Sponge(BYTE *S, const BYTE *P)
{
	QWORD b = 1600;
	for (QWORD i = 0; i < b >> 3; i++)
	{
		S[i] ^= P[i];
	}
	KeccakF(6, S, S);
}
void PAD101(BYTE *S, QWORD x, QWORD &position)
{
	Memory::fill(S + position, 0, x - position);
	S[x - 1] |= 0x80;
}

Cryptography::SHA3::SHA3(BYTE type): Cryptography::MessageDigest::MessageDigest(200, 200, nullptr), function(type)
{
	Memory::fill(this->block.address, 0, this->block.length);
	Memory::fill(this->digest.address, 0, this->digest.length);
}
bool Cryptography::SHA3::appendix(Memory::string &, QWORD &)
{
	return false;
}
void Cryptography::SHA3::transform(Memory::string &)
{
}
void Cryptography::SHA3::update(const void *data, QWORD len)
{
	QWORD b = 1600;
	QWORD c = SHA3FUNCTIONS[this->function] << 1;
	QWORD size = (b - c) >> 3;
	this->length += len << 3;
	const char *buf = (const char *) data;
	while (len)
	{
		QWORD copy = size - this->position;
		copy = len < copy ? len : copy;
		Memory::copy(this->block.address + this->position, buf, copy);
		buf += copy;
		this->position += copy;
		len -= copy;
		if (this->position >= size)
		{
			this->position = 0;
			Sponge(this->digest.address, this->block.address);
		}
	}
}
Memory::string Cryptography::SHA3::finally()
{
	QWORD b = 1600;
	QWORD c = SHA3FUNCTIONS[this->function] << 1;
	QWORD size = (b - c) >> 3;
	Memory::string blk = this->block;
	Memory::string out = this->digest;
	QWORD pos = this->position;
	blk[pos++] = 6;
	PAD101(blk.address, size, pos);
	Sponge(out.address, blk.address);
	out.resize(SHA3FUNCTIONS[this->function] >> 3);
	return out;
}