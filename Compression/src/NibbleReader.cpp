#include "definitions.h"

Compression::NibbleReader::NibbleReader(const Memory::string &stream): stream(stream)
{
}

QWORD Compression::NibbleReader::read(BYTE bits)
{
	if (bits <= 64)
	{
		QWORD ret = 0;
		QWORD mov = 64;
		while (bits)
		{
			QWORD idx = this->position >> 3;
			QWORD btx = this->position & 0x7;

			ret >>= 1;
			ret |= ((this->stream[idx] >> btx) & 1ULL) << 63;

			this->position++;
			mov--;
			bits--;
		}
		return ret >> mov;
	}
	throw Exception::exception("Bitwise count is too large");
}

BYTE Compression::NibbleReader::boundary()
{
	QWORD idx = this->position >> 3;
	QWORD btx = this->position & 0x7;
	BYTE ret = 0;
	if (btx)
	{
		ret = this->stream[idx] >> btx;
		this->position = (idx + 1) << 3;
	}
	return ret;
}