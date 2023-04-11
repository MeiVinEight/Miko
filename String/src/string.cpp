#include "definitions.h"

String::string::string()
{
	this->address[0] = 0;
}
String::string::string(const char *str): string(str, String::length(str))
{
}
String::string::string(const void *str, QWORD len): address(len)
{
	Memory::copy(this->address.address, str, len);
}
String::string::string(const Memory::string &str): string((char *) str.address, str.length)
{
}
String::string::string(const Memory::string &str, QWORD len): string((char *) str.address, len)
{
}
String::string::string(const String::string &str) = default;
String::string::string(String::string &&move) noexcept = default;
String::string::~string() = default;
String::string &String::string::operator=(const char *str)
{
	return (*this) = (String::string)str;
}
String::string &String::string::operator=(const String::string &) & = default;
String::string &String::string::operator=(String::string &&move) & noexcept = default;
bool String::string::operator==(const char *str) const
{
	QWORD len = String::length(str);
	if (len == this->length())
	{
		return Memory::compare(this->address.address, str, len);
	}
	return false;
}
bool String::string::operator==(const String::string &other) const
{
	if (&other != this)
	{
		if (this->length() == other.length())
		{
			return Memory::compare(this->address.address, other.address.address, this->length());
		}
		return false;
	}
	return true;
}
bool String::string::operator!=(const String::string &str) const
{
	return !((*this) == str);
}
char &String::string::operator[](QWORD idx) const
{
	return (char &)this->address[idx];
}
String::string &String::string::operator+=(const char *str) &
{
	QWORD len = String::length(str);
	this->address.resize(this->length() + len);
	Memory::copy(this->address.address + this->length(), str, len);
	return *this;
}
String::string &String::string::operator+=(const String::string &str) &
{
	this->address.resize(this->length() + str.length());
	Memory::copy(this->address.address + this->length(), str.address.address, str.length());
	return *this;
}
String::string String::string::operator+(const char *str) const
{
	String::string lval(*this);
	return lval += str;
}
String::string String::string::operator+(const String::string &str) const
{
	String::string lval(*this);
	return lval += str;
}
String::string::operator bool() const
{
	return this->length();
}
QWORD String::string::length() const
{
	return this->address.length;
}
String::string String::string::lower() const
{
	String::string ret = *this;
	for (QWORD i = 0; i < ret.length(); i++)
	{
		if (ret[i] >= 'A' && ret[i] <= 'Z')
		{
			ret[i] += 0x20;
		}
	}
	return ret;
}
String::string String::string::upper() const
{
	String::string ret = *this;
	for (QWORD i = 0; i < ret.length(); i++)
	{
		if (ret[i] >= 'a' && ret[i] <= 'z')
		{
			ret[i] -= 0x20;
		}
	}
	return ret;
}