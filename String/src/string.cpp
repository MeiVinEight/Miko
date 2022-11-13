#define STRING_SHARED
#include <sstring.h>

QWORD String::length(const char *str)
{
	const char *s = str;
	while (*s++);
	return s - str - 1;
}

String::string::string()
{
	this->address[0] = 0;
}

String::string::string(const char *str): address(Memory::string(String::length(str) + 1))
{
	this->length = this->address.length - 1;
	Memory::copy(this->address.address, str, this->length);
}

String::string::string(const String::string &) = default;

String::string::string(String::string &&move): address((Memory::string &&)move.address), length(move.length)
{
}

String::string::~string() = default;

String::string &String::string::operator=(const String::string &) = default;

String::string &String::string::operator=(String::string &&move)
{
	if (&move != this)
	{
		this->address = (Memory::string &&)move.address;
		this->length = move.length;
	}
	return *this;
}

bool String::string::operator==(const String::string &&other) const
{
	if (this->length == other.length)
	{
		return Memory::compare(this->address.address, other.address.address, this->length);
	}
	return false;
}