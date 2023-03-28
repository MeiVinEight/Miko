#include "SHA256.h"

DWORD SHA256K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void CalculateSHA256(BYTE *block, BYTE *digest)
{
	DWORD *HH = (DWORD *) digest;
	DWORD A = HH[0];
	DWORD B = HH[1];
	DWORD C = HH[2];
	DWORD D = HH[3];
	DWORD E = HH[4];
	DWORD F = HH[5];
	DWORD G = HH[6];
	DWORD H = HH[7];
	DWORD W[64];
	for (DWORD t = 0; t < 64; t++)
	{
		W[t] = GetAsBEndian(4, block + 4 * (t & 0xF));
		if (t > 15)
		{
			W[t] = Sigma1256(W[t - 2]) + W[t - 7] + Sigma0256(W[t - 15]) + W[t - 16];
		}
		DWORD T1 = H + SIGMA1256(E) + Ch(E, F, G) + SHA256K[t] + W[t];
		DWORD T2 = SIGMA0256(A) + Maj(A, B, C);
		H = G;
		G = F;
		F = E;
		E = D + T1;
		D = C;
		C = B;
		B = A;
		A = T1 + T2;
	}
	HH[0] += A;
	HH[1] += B;
	HH[2] += C;
	HH[3] += D;
	HH[4] += E;
	HH[5] += F;
	HH[6] += G;
	HH[7] += H;
}

Crypto::SHA256::SHA256(): MessageDigest(Crypto::MessageDigest::BLOCK_SIZE_32, 32, &CalculateSHA256)
{
	DWORD H[8] = {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};
	Memory::copy(this->digest.address, H, this->digest.length);
}
bool Crypto::SHA256::appendix(Memory::string &block, QWORD &position)
{
	return Appendix32(this->length, block, position, &SaveAsBEndian);
}
void Crypto::SHA256::transform(Memory::string &digest)
{
	Transform32(digest, &SaveAsBEndian);
}
