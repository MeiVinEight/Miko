#include "definitions.h"

String::URL::URL() = default;
String::URL::~URL() noexcept = default;
void String::URL::resolve(const String::string &url)
{
	DWORD idx = url.search(':');
	if (idx >= url.length())
		throw Memory::exception(Memory::ERRNO_INVALID_PARAMETER);
	this->scheme = String::string(url.address, idx - 1);
	this->specific = String::string(url.address.address + idx, url.length() - idx);
}