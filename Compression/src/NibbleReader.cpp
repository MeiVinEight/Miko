#include "definitions.h"

Compression::NibbleReader::NibbleReader(const Memory::string &stream): stream(stream)
{
}

QWORD Compression::NibbleReader::read(BYTE bits)
{
	if (bits <= 64)
	{
		if (this->position + bits <= this->stream.length * 8)
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
		throw Exception::exception("EOF");
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

void Compression::NibbleReader::seek(QWORD pos)
{
	if (pos > this->stream.length * 8)
	{
		throw Exception::exception("EOF");
	}
	this->position = pos;
}

void Compression::NibbleReader::skip(QWORD len)
{
	if (this->position + len > this->stream.length * 8)
	{
		throw Exception::exception("EOF");
	}
	this->position += len;
}