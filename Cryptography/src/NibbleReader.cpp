#include "Keccak.h"

QWORD NibbleReader::read(BYTE bits)
{
	QWORD ret = 0;
	QWORD mov = 64;
	while (bits--)
	{
		QWORD idx = this->position >> 3;
		QWORD btx = this->position & 0x7;

		ret >>= 1;
		ret |= ((this->stream[idx] >> btx) & 1ULL) << 63;

		this->position++;
		mov -= !!mov;
	}
	return ret >> mov;
}
void NibbleReader::write(QWORD x, BYTE bits)
{
	while (bits--)
	{
		QWORD idx = this->position >> 3;
		QWORD btx = this->position & 0x7;
		BYTE msk = 0xFF;
		msk ^= 1 << btx;
		this->stream[idx] &= msk;
		this->stream[idx] |= ((x & 1) << btx);
		x >>= 1;
		this->position++;
	}
}