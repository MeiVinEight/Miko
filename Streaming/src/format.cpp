#include <format.h>
#include <endian.h>
#include <hexadecimal.h>
#include <streaming.h>

Streaming::format::format(Streaming::stream *stream): stream(stream)
{
}
DWORD Streaming::format::read(void *b, DWORD len)
{
	DWORD readed = 0;
	if (len)
	{
		char *buf = (char *) b;
		if (~this->temporary)
		{
			char temp = (char) (this->temporary & 0xFF);
			Memory::copy(buf, &temp, 1);
			len--;
			buf++;
			readed++;
			this->temporary = 0xFFFFFFFF;
		}
		DWORD x = 0xFFFFFFFF;
		while (len && x)
		{
			x = this->stream->read(buf, len);
			len -= x;
			readed += x;
			buf += x;
		}
	}
	return readed;
}
DWORD Streaming::format::write(const void *b, DWORD len)
{
	QWORD length = len;
	const BYTE *buf = (const BYTE *) b;
	while (length)
	{
		DWORD written = this->stream->write(buf, length);
		length -= written;
		buf += written;
	}
	return len;
}
void Streaming::format::flush()
{
	this->stream->flush();
}
QWORD Streaming::format::available()
{
	return this->stream->available();
}
Streaming::format &Streaming::format::operator>>(const Memory::string &data)
{
	this->read(data.address, data.length);
	return *this;
}
Streaming::format &Streaming::format::operator<<(const Memory::string &data)
{
	this->write(data.address, data.length);
	return *this;
}
Streaming::format &Streaming::format::operator>>(char &x)
{
	this->read(&x, 1);
	return *this;
}
Streaming::format &Streaming::format::operator>>(int &x)
{
	QWORD x1 = 0;
	(*this) >> x1;
	x = (int) x1;
	return *this;
}
Streaming::format &Streaming::format::operator>>(DWORD &x)
{
	QWORD x1 = 0;
	(*this) >> x1;
	x = x1;
	return *this;
}
Streaming::format &Streaming::format::operator>>(QWORD &x)
{
	String::string num;
	(*this) >> num;
	x = String::integer(num);
	return *this;
}
Streaming::format &Streaming::format::operator>>(String::string &str)
{
	Memory::string buf(16);
	QWORD len = 0;
	bool whitespace = true;
	bool ava = true;
	bool continu = true;
	while (ava && continu)
	{
		BYTE c;
		ava &= this->read(&c, 1) != 0;
		if (ava)
		{
			whitespace &= c <= 0x20;
			continu = whitespace || c > 0x20;
			this->temporary = continu ? 0xFFFFFFFF : (c & 0xFF);
			if (c > 0x20)
			{
				if (len >= buf.length)
				{
					buf.resize(buf.length * 2);
				}
				buf[len++] = c;
			}
		}
	}
	str = String::string(buf, len);
	return *this;
}
Streaming::format &Streaming::format::operator<<(bool b)
{
	if (b)
	{
		(*this) << "true";
	}
	else
	{
		(*this) << "false";
	}
	return *this;
}
Streaming::format &Streaming::format::operator<<(char x)
{
	this->write(&x, 1);
	return *this;
}
Streaming::format &Streaming::format::operator<<(int x)
{
	(*this) << String::stringify(x);
	return *this;
}
Streaming::format &Streaming::format::operator<<(DWORD x)
{
	(*this) << (QWORD) x;
	return *this;
}
Streaming::format &Streaming::format::operator<<(QWORD x)
{
	(*this) << String::stringify(x);
	return *this;
}
Streaming::format &Streaming::format::operator<<(void *x)
{
	(*this) << String::stringify(x);
	return *this;
}
Streaming::format &Streaming::format::operator<<(const char *str)
{
	this->write(str, String::length(str));
	return *this;
}
Streaming::format &Streaming::format::operator<<(const String::string &str)
{
	(*this) << str.address;
	return *this;
}
Streaming::format &Streaming::format::operator<<(const Memory::exception &exce)
{
	Streaming::format &cout = *this;
	Memory::string err(4);
	Memory::BE::set(exce.code, err.address, 4);
	cout << '[' << exce.type << "][" << Hexadecimal::format(err) << "] " << exce.message << Streaming::LF;
	for (DWORD i = 0; i < exce.count; i++)
	{
		cout << exce.stack[i].offset;
		cout << " [";
		cout << exce.stack[i].library;
		cout << '+';
		cout << Hexadecimal::stringify((QWORD) exce.stack[i].offset - (QWORD) exce.stack[i].module);
		cout << ']';
		if (exce.stack[i].function.length)
		{
			cout << " (";
			cout << exce.stack[i].function;
			cout << '+';
			cout << Hexadecimal::stringify((QWORD) exce.stack[i].offset - (QWORD) exce.stack[i].address);
			cout << ')';
		}
		cout << Streaming::LF;
	}
	return *this;
}
Streaming::format &Streaming::format::operator<<(void (*f)(Streaming::stream *))
{
	f(this);
	return *this;
}