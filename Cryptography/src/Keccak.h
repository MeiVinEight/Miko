#ifndef KECCAK_H
#define KECCAK_H

#include "definitions.h"

class NibbleReader
{
	public:
	BYTE *stream = nullptr;
	QWORD position = 0;

	QWORD read(BYTE);
	void write(QWORD, BYTE);
};
class SPONGE
{
	public:
	QWORD A[5][5] = {};
	BYTE L = 0; // w = 2 << L, b = 25 * w

	void theta();
	void rho();
	void pi();
	void chi();
	void iota(QWORD);
};

#endif //KECCAK_H
