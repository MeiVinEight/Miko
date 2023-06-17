#include <stream.h>

DWORD Streaming::stream::read(const Memory::string &buf)
{
	return this->read(buf.address, buf.length);
}
DWORD Streaming::stream::read(const Memory::string &buf, DWORD len)
{
	return this->read(buf.address, len);
}
DWORD Streaming::stream::write(const Memory::string &buf)
{
	return this->write(buf.address, buf.length);
}
DWORD Streaming::stream::write(const Memory::string &buf, DWORD len)
{
	return this->write(buf.address, len);
}