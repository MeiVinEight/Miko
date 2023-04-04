#include "Keccak.h"

QWORD RC_CONSTANTS[24] = {
	0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808AULL,
	0x8000000080008000ULL, 0x000000000000808BULL, 0x0000000080000001ULL,
	0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008AULL,
	0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000AULL,
	0x000000008000808BULL, 0x800000000000008BULL, 0x8000000000008089ULL,
	0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
	0x000000000000800AULL, 0x800000008000000AULL, 0x8000000080008081ULL,
	0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL,
};

void KeccakTHETA(QWORD A[5][5])
{
	QWORD C0 = A[0][0] ^ A[0][1] ^ A[0][2] ^ A[0][3] ^ A[0][4];
	QWORD C1 = A[1][0] ^ A[1][1] ^ A[1][2] ^ A[1][3] ^ A[1][4];
	QWORD C2 = A[2][0] ^ A[2][1] ^ A[2][2] ^ A[2][3] ^ A[2][4];
	QWORD C3 = A[3][0] ^ A[3][1] ^ A[3][2] ^ A[3][3] ^ A[3][4];
	QWORD C4 = A[4][0] ^ A[4][1] ^ A[4][2] ^ A[4][3] ^ A[4][4];
	QWORD D0 = C4 ^ ROTL64(C1, 1);
	QWORD D1 = C0 ^ ROTL64(C2, 1);
	QWORD D2 = C1 ^ ROTL64(C3, 1);
	QWORD D3 = C2 ^ ROTL64(C4, 1);
	QWORD D4 = C3 ^ ROTL64(C0, 1);
	for (QWORD y = 0; y < 5; y++)
	{
		A[0][y] ^= D0;
		A[1][y] ^= D1;
		A[2][y] ^= D2;
		A[3][y] ^= D3;
		A[4][y] ^= D4;
	}
}
void KeccakRHO(QWORD A[5][5])
{
	QWORD x = 1;
	QWORD y = 0;
	for (QWORD t = 0; t < 24; t++)
	{
		A[x][y] = ROTL64(A[x][y], ((t + 1) * (t + 2) / 2) % 64);
		QWORD z = (2 * x + 3 * y) % 5;
		x = y;
		y = z;
	}
}
void KeccakPI(QWORD A[5][5])
{
	QWORD tmp = A[0][2];
	A[0][2] = A[1][0];
	A[1][0] = A[1][1];
	A[1][1] = A[4][1];
	A[4][1] = A[2][4];
	A[2][4] = A[4][2];
	A[4][2] = A[0][4];
	A[0][4] = A[2][0];
	A[2][0] = A[2][2];
	A[2][2] = A[3][2];
	A[3][2] = A[4][3];
	A[4][3] = A[3][4];
	A[3][4] = A[0][3];
	A[0][3] = A[4][0];
	A[4][0] = A[4][4];
	A[4][4] = A[1][4];
	A[1][4] = A[3][1];
	A[3][1] = A[1][3];
	A[1][3] = A[0][1];
	A[0][1] = A[3][0];
	A[3][0] = A[3][3];
	A[3][3] = A[2][3];
	A[2][3] = A[1][2];
	A[1][2] = A[2][1];
	A[2][1] = tmp;
}
void KeccakCHI(QWORD A[5][5])
{
	for (QWORD y = 0; y < 5; y++)
	{
		QWORD ay0 = A[0][y];
		QWORD ay1 = A[1][y];
		QWORD ay2 = A[2][y];
		QWORD ay3 = A[3][y];
		QWORD ay4 = A[4][y];
		A[0][y] = ay0 ^ ((~ay1) & ay2);
		A[1][y] = ay1 ^ ((~ay2) & ay3);
		A[2][y] = ay2 ^ ((~ay3) & ay4);
		A[3][y] = ay3 ^ ((~ay4) & ay0);
		A[4][y] = ay4 ^ ((~ay0) & ay1);
	}
}
void KeccakIOTA(QWORD A[5][5], QWORD ir)
{
	A[0][0] ^= RC_CONSTANTS[ir];
}
void KeccakP(BYTE *S)
{
	QWORD A[5][5];
	for (QWORD y = 0; y < 5; y++)
	{
		for (QWORD x = 0; x < 5; x++)
		{
			A[x][y] = GetAsLEndian(8, S + ((x + (5 * y)) << 3));
		}
	}
	for (QWORD ir = 0; ir < 24; ir++)
	{
		KeccakTHETA(A);
		KeccakRHO(A);
		KeccakPI(A);
		KeccakCHI(A);
		KeccakIOTA(A, ir);
	}
	for (QWORD y = 0; y < 5; y++)
	{
		for (QWORD x = 0; x < 5; x++)
		{
			SaveAsLEndian(A[x][y], 8, S + ((x + (5 * y)) << 3));
		}
	}
}
void KeccakF(BYTE *S)
{
	return KeccakP(S);
}
void SPONGE(BYTE *S, const BYTE *P)
{
	QWORD b = 1600;
	for (QWORD i = 0; i < b >> 3; i++)
	{
		S[i] ^= P[i];
	}
	KeccakF(S);
}
void PAD101(BYTE *S, QWORD x, QWORD &position)
{
	Memory::fill(S + position, 0, x - position);
	S[x - 1] |= 0x80;
}