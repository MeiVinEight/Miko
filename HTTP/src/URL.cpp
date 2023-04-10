#include "definitions.h"

HTTP::URL::URL(const String::string &url): String::URL(url)
{
	String::string protocol = this->scheme.lower();
	if (protocol != "http" && protocol != "https")
	{
		// TODO Unknown protocol
	}
	if (protocol == "https")
		this->port = 443;

	DWORD idx = 2;
	while (idx < this->specific.length() && this->specific[idx++] != '/');
	idx--;
	String::string hostport(this->specific.address.address + 2, idx - 2);
	this->path = String::string(this->specific.address.address + idx, this->specific.length() - idx);
	idx = 0;
	while (idx < hostport.length() && hostport[idx++] != ':');
	this->host = String::string(hostport.address, idx - 1);
	String::string ports(hostport.address.address + idx, hostport.length() - idx);
	if (ports)
	{
		this->port = String::integer(ports);
	}
}