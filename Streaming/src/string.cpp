#include <stringstream.h>

DWORD Streaming::string::read(void *b, DWORD length)
{
	QWORD ava = this->available();
	length = (length > ava) ? ava : length;
	Memory::copy(b, this->address.address + this->position, length);
	this->position += length;
	return length;
}
DWORD Streaming::string::write(const void *b, DWORD length)
{
	this->address.resize(this->position + length);
	Memory::copy(this->address.address + this->position, b, length);
	return length;
}
void Streaming::string::flush()
{
}
QWORD Streaming::string::available()
{
	return this->address.length - this->position;
}