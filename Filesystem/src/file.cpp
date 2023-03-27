#include "definitions.h"

Streaming::file::file(QWORD fdVal): object(fdVal)
{
}
Streaming::file::file(const String::string &path): file(Filesystem::open(path, Filesystem::OF_READWRITE))
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
void Streaming::file::read(void *b, DWORD len)
{
	if (~this->object)
	{
		char *buf = (char *) b;
		while (len)
		{
			DWORD readed = Filesystem::read(this->object, buf, len);
			if (!readed)
			{
				throw Exception::exception("EOF");
			}
			buf += readed;
			len -= readed;
		}
		return;
	}
	throw Exception::exception("File closed");
}
Streaming::file &Streaming::file::operator>>(const Memory::string &str)
{
	this->read(str.address, str.length);
	return *this;
}
void Streaming::file::write(const void *b, DWORD len)
{
	if (~this->object)
	{
		const char *buf = (const char *) b;
		while (len)
		{
			DWORD written = Filesystem::write(this->object, buf, len);
			buf += written;
			len -= written;
		}
		return;
	}
	throw Exception::exception("File closed");
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
		HANDLE handle = (HANDLE) this->object;
		DWORD type = GetFileType(handle);
		switch (type)
		{
			case FILE_TYPE_DISK:
			{
				LARGE_INTEGER distance, pos;
				distance.QuadPart = 0;
				if (SetFilePointerEx(handle, distance, &pos, FILE_CURRENT))
				{
					QWORD current = pos.QuadPart;
					LARGE_INTEGER filesize;
					if (GetFileSizeEx(handle, &filesize))
					{
						return filesize.QuadPart - current;
					}
				}
				break;
			}
		}
	}
	return -1;
}
void Streaming::file::seek(QWORD offset) const
{
	Filesystem::seek(this->object, offset, FILE_BEGIN);
}
void Streaming::file::close()
{
	if (~this->object)
	{
		Filesystem::close(this->object);
		this->object = Filesystem::FILE_ERROR;
	}
}