#include "definitions.h"

void HTTP::URL::resolve(const String::string &url)
{
	this->String::URL::resolve(url);
	String::string protocol = this->scheme.lower();
	if (protocol != "http" && protocol != "https")
		throw Memory::exception(Memory::ERRNO_INVALID_PARAMETER);

	if (this->specific[0] != '/' || this->specific[1] != '.')
		throw Memory::exception(Memory::ERRNO_INVALID_PARAMETER);

	if (protocol == "https")
		this->port = 443;

	String::string http(this->specific.address.address + 2, this->specific.length() - 2);
	DWORD idx = http.search('/');
	idx = (idx > http.length()) ? http.length() : idx;
	String::string hostport(http.address, idx);
	this->path = String::string(http.address, http.length() - idx);
	if (!this->path.length())
		this->path = "/";

	idx = hostport.search(':');
	idx = (idx > hostport.length()) ? hostport.length() : idx;
	this->host = String::string(hostport.address, idx);
	if (idx < hostport.length())
	{
		idx++;
		String::string ports(hostport.address.address + idx, hostport.length() - idx);
		if (!ports)
			throw Memory::exception(Memory::ERRNO_INVALID_PARAMETER);
		this->port = String::integer(ports) & 0xFFFF;
		this->special = true;
	}
}