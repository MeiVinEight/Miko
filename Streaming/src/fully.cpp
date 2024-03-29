#include <fully.h>

Streaming::fully::fully(Streaming::stream *delegating): stream(delegating)
{
}
DWORD Streaming::fully::read(void *b, DWORD length)
{
	DWORD ret = length;
	char *buf = (char *) b;
	while (length)
	{
		DWORD readed = this->stream->read(buf, length);
		buf += readed;
		length -= readed;
	}
	return ret;
}
DWORD Streaming::fully::read(const Memory::string &buf)
{
	return this->read(buf.address,buf.length);
}
DWORD Streaming::fully::read(const Memory::string &buf, DWORD len)
{
	return this->read(buf.address, len);
}
DWORD Streaming::fully::write(const void *b, DWORD length)
{
	DWORD ret = length;
	const char *buf = (const char *) b;
	while (length)
	{
		DWORD written = this->stream->write(buf, length);
		buf += written;
		length -= written;
	}
	return ret;
}
DWORD Streaming::fully::write(const Memory::string &buf)
{
	return this->write(buf.address, buf.length);
}
DWORD Streaming::fully::write(const Memory::string &buf, DWORD len)
{
	return this->write(buf.address, len);
}
void Streaming::fully::flush()
{
	this->stream->flush();
}
QWORD Streaming::fully::available()
{
	return this->stream->available();
}