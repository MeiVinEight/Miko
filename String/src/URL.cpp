#include "definitions.h"

String::URL::URL(const String::string &url): location(url)
{
	DWORD idx = 0;
	while (idx < url.length() && url[idx++] != ':');
	this->scheme = String::string(url.address, idx - 1);
	this->specific = String::string(url.address.address + idx, url.length() - idx);
}