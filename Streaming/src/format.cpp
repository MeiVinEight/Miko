#include <format.h>

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
		while (len)
		{
			DWORD x = this->stream->read(buf, len);
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
	QWORD x1 = 0;
	bool sign = true;
	bool neg = false;
	bool whitespace = false;
	bool valid = false;
	char c;
	bool continu = true;
	while (continu)
	{
		(*this) >> c;
		whitespace |= (c < 0x21);
		if (c == '+' || c == '-')
		{
			continu = sign;
			if (sign)
			{
				sign = false;
				neg = c == '-';
			}
		}
		else if (c >= '0' && c <= '9')
		{
			valid = true;
			int d = c - '0';
			x1 *= 10;
			x1 += d;
		}
		else
		{
			continu = !whitespace;
		}
		this->temporary = continu ? 0xFFFFFFFF : (c & 0xFF);
	}
	x1 = neg ? -x1 : x1;
	valid ? (x = x1) : 0;
	return *this;
}
Streaming::format &Streaming::format::operator>>(String::string &str)
{
	Memory::string buf(16);
	QWORD len = 0;
	bool whitespace = true;
	bool continu = true;
	while (continu)
	{
		BYTE c;
		(*this) >> *((char *) &c);
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
Streaming::format &Streaming::format::operator<<(void (*f)(Streaming::stream *))
{
	f(this);
	return *this;
}