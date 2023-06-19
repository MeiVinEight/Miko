#include "rotate.h"

QWORD MASK(BYTE x)
{
	x &= 0x3F;
	x = 64 - x;
	return (0xFFFFFFFFFFFFFFFF << x) >> x;
}
QWORD ROTL(QWORD x, BYTE n, BYTE w) { return ((x << (n % w)) | (x >> (w - (n % w)))) & MASK(w); }
QWORD ROTR(QWORD x, BYTE n, BYTE w) { return ((x >> (n % w)) | (x << (w - (n % w)))) & MASK(w); }
QWORD ROTL32(QWORD X, BYTE n) { return ROTL(X, n, 32); }
QWORD ROTR32(QWORD X, BYTE n) { return ROTR(X, n, 32); }
QWORD ROTL64(QWORD X, BYTE n) { return ROTL(X, n, 64); }
QWORD ROTR64(QWORD X, BYTE n) { return ROTR(X, n, 64); }