#include <filestream.h>
#include <exception.h>


Streaming::file::file(QWORD fdVal): object(fdVal)
{
}
Streaming::file::file(const String::string &path): file(Filesystem::open(path, Filesystem::FA_READ_DATA | Filesystem::FA_WRITE_DATA))
{
}
Streaming::file::file(Streaming::file &&move) noexcept: object(move.object)
{
	move.object = Filesystem::FILE_ERROR;
}
Streaming::file::~file()
{
	this->Streaming::file::close();
}
Streaming::file &Streaming::file::operator=(Streaming::file &&move) noexcept
{
	if (this != &move)
	{
		this->close();
		this->object = move.object;
		move.object = Filesystem::FILE_ERROR;
	}
	return *this;
}
DWORD Streaming::file::read(void *b, DWORD len)
{
	if (~this->object)
	{
		return Filesystem::read(this->object, (char *) b, len);
	}
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
Streaming::file &Streaming::file::operator>>(const Memory::string &str)
{
	this->read(str.address, str.length);
	return *this;
}
DWORD Streaming::file::write(const void *b, DWORD len)
{
	if (~this->object)
	{
		return Filesystem::write(this->object, (const char *) b, len);
	}
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
Streaming::file &Streaming::file::operator<<(const Memory::string &str)
{
	this->write(str.address, str.length);
	return *this;
}
void Streaming::file::flush()
{
	if (~this->object)
	{
		Filesystem::flush(this->object);
	}
}
QWORD Streaming::file::available()
{
	if (~this->object)
	{
		return Filesystem::available(this->object);
	}
	return -1;
}
void Streaming::file::seek(QWORD offset) const
{
	if (~this->object)
	{
		Filesystem::seek(this->object, offset, Filesystem::SEEK_BEGIN);
	}
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
void Streaming::file::close()
{
	if (~this->object)
	{
		Filesystem::close(this->object);
		this->object = Filesystem::FILE_ERROR;
	}
}