#include "definitions.h"

void Streaming::stream::read(const Memory::string &buf)
{
	this->read(buf.address, buf.length);
}
void Streaming::stream::read(const Memory::string &buf, DWORD len)
{
	this->read(buf.address, len);
}
void Streaming::stream::write(const Memory::string &buf)
{
	this->write(buf.address, buf.length);
}
void Streaming::stream::write(const Memory::string &buf, DWORD len)
{
	this->write(buf.address, len);
}