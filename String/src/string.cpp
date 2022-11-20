#define STRING_SHARED
#include <sstring.h>

QWORD String::length(const void *str)
{
	const char *s = (const char *)str;
	while (*s++);
	return s - (const char *)str - 1;
}

String::string::string()
{
	this->address[0] = 0;
}

String::string::string(const void *str): String::string::string(str, String::length(str))
{
}

String::string::string(const void *str, QWORD len): address(len + 1), length(len)
{
	Memory::copy(this->address, str, len);
	this->address[len] = 0;
}

String::string::string(const String::string &) = default;

String::string::string(String::string &&move): address((Memory::string &&)move.address), length(move.length)
{
	move.length = 0;
}

String::string::~string() = default;

String::string &String::string::operator=(const void *str)
{
	return (*this) = (String::string)str;
}

String::string &String::string::operator=(const String::string &) & = default;

String::string &String::string::operator=(String::string &&move) &
{
	if (&move != this)
	{
		this->address = (Memory::string &&)move.address;
		this->length = move.length;
	}
	return *this;
}

bool String::string::operator==(const String::string &other) const
{
	if (&other != this)
	{
		if (this->length == other.length)
		{
			return Memory::compare(this->address, other.address, this->length);
		}
		return false;
	}
	return true;
}

bool String::string::operator==(const void *str) const
{
	QWORD len = String::length(str);
	if (len == this->length)
	{
		return Memory::compare(this->address, str, len);
	}
	return false;
}

char &String::string::operator[](QWORD idx) const
{
	return (char &)this->address[idx];
}

String::string &String::string::operator+=(const String::string &str) &
{
	this->address.ensure(this->length + str.length + 1);
	Memory::copy(this->address + this->length, str.address, str.length);
	this->length += str.length;
	(*this)[this->length] = 0;
	return *this;
}

String::string &String::string::operator+=(const void *str) &
{
	QWORD len = String::length(str);
	this->address.ensure(this->length + len + 1);
	Memory::copy(this->address + this->length, str, len);
	this->length += len;
	(*this)[this->length] = 0;
	return *this;
}

String::string String::string::operator+(const String::string &str) const
{
	String::string lval(*this);
	return lval += str;
}

String::string String::string::operator+(const void *str) const
{
	String::string lval(*this);
	return lval += str;
}