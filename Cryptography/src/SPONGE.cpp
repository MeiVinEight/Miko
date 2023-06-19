#include "Keccak.h"
#include "definitions.h"

void SPONGE::theta()
{
	BYTE w = 1 << this->L;
	QWORD C0 = this->A[0][0] ^ this->A[0][1] ^ this->A[0][2] ^ this->A[0][3] ^ this->A[0][4];
	QWORD C1 = this->A[1][0] ^ this->A[1][1] ^ this->A[1][2] ^ this->A[1][3] ^ this->A[1][4];
	QWORD C2 = this->A[2][0] ^ this->A[2][1] ^ this->A[2][2] ^ this->A[2][3] ^ this->A[2][4];
	QWORD C3 = this->A[3][0] ^ this->A[3][1] ^ this->A[3][2] ^ this->A[3][3] ^ this->A[3][4];
	QWORD C4 = this->A[4][0] ^ this->A[4][1] ^ this->A[4][2] ^ this->A[4][3] ^ this->A[4][4];
	QWORD D0 = C4 ^ ROTL(C1, 1, w);
	QWORD D1 = C0 ^ ROTL(C2, 1, w);
	QWORD D2 = C1 ^ ROTL(C3, 1, w);
	QWORD D3 = C2 ^ ROTL(C4, 1, w);
	QWORD D4 = C3 ^ ROTL(C0, 1, w);
	for (QWORD y = 0; y < 5; y++)
	{
		this->A[0][y] ^= D0;
		this->A[1][y] ^= D1;
		this->A[2][y] ^= D2;
		this->A[3][y] ^= D3;
		this->A[4][y] ^= D4;
	}
}
void SPONGE::rho()
{
	BYTE w = 1 << this->L;
	QWORD x = 1;
	QWORD y = 0;
	for (QWORD t = 0; t < 24; t++)
	{
		this->A[x][y] = ROTL(this->A[x][y], ((t + 1) * (t + 2) / 2) % w, w);
		QWORD z = (2 * x + 3 * y) % 5;
		x = y;
		y = z;
	}
}
void SPONGE::pi()
{
	QWORD tmp = this->A[0][2];
	this->A[0][2] = this->A[1][0];
	this->A[1][0] = this->A[1][1];
	this->A[1][1] = this->A[4][1];
	this->A[4][1] = this->A[2][4];
	this->A[2][4] = this->A[4][2];
	this->A[4][2] = this->A[0][4];
	this->A[0][4] = this->A[2][0];
	this->A[2][0] = this->A[2][2];
	this->A[2][2] = this->A[3][2];
	this->A[3][2] = this->A[4][3];
	this->A[4][3] = this->A[3][4];
	this->A[3][4] = this->A[0][3];
	this->A[0][3] = this->A[4][0];
	this->A[4][0] = this->A[4][4];
	this->A[4][4] = this->A[1][4];
	this->A[1][4] = this->A[3][1];
	this->A[3][1] = this->A[1][3];
	this->A[1][3] = this->A[0][1];
	this->A[0][1] = this->A[3][0];
	this->A[3][0] = this->A[3][3];
	this->A[3][3] = this->A[2][3];
	this->A[2][3] = this->A[1][2];
	this->A[1][2] = this->A[2][1];
	this->A[2][1] = tmp;
}
void SPONGE::chi()
{
	for (QWORD y = 0; y < 5; y++)
	{
		QWORD ay0 = this->A[0][y];
		QWORD ay1 = this->A[1][y];
		QWORD ay2 = this->A[2][y];
		QWORD ay3 = this->A[3][y];
		QWORD ay4 = this->A[4][y];
		this->A[0][y] = ay0 ^ ((~ay1) & ay2);
		this->A[1][y] = ay1 ^ ((~ay2) & ay3);
		this->A[2][y] = ay2 ^ ((~ay3) & ay4);
		this->A[3][y] = ay3 ^ ((~ay4) & ay0);
		this->A[4][y] = ay4 ^ ((~ay0) & ay1);
	}
}
void SPONGE::iota(QWORD ir)
{
	QWORD RC = 0;
	for (BYTE j = 0; j <= this->L; j++)
	{
		RC |= ((QWORD) KECCAKRC[(j + 7 * ir) % 255]) << ((1ULL << j) - 1);
	}
	this->A[0][0] ^= RC;
}