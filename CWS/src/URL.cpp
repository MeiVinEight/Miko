#include <winerr.h>
#include <exception.h>
#include <WSURL.h>

void CWS::URL::resolve(const String::string &url)
{
	this->String::URL::resolve(url);
	String::string protocol = this->scheme.lower();
	if (protocol != "ws" && protocol != "wss")
		throw Memory::exception(ERROR_INVALID_PARAMETER, Memory::DOSERROR);

	if (this->specific[0] != '/' && this->specific[1] != '/')
		throw Memory::exception(ERROR_INVALID_PARAMETER, Memory::DOSERROR);

	if (protocol == "ws")
		this->ID = 80;

	String::string ws(this->specific.address.address + 2, this->specific.length() - 2);
	DWORD idx = ws.search('/');
	idx = (idx > ws.length()) ? ws.length() : idx;
	String::string hostport(ws.address, idx);
	this->path = String::string(ws.address.address + idx, ws.length() - idx);
	if (!this->path)
		this->path = "/";

	idx = hostport.search(':');
	idx = (idx > hostport.length()) ? hostport.length() : idx;
	this->address = String::string(hostport.address, idx);
	if (idx < hostport.length())
	{
		idx++;
		String::string port(hostport.address.address + idx, hostport.length() - idx);
		if (!port)
			throw Memory::exception(ERROR_INVALID_PARAMETER, Memory::DOSERROR);
		QWORD id = String::integer(port);
		if (id > 0xFFFF || id == 0)
			throw Memory::exception(ERROR_INVALID_PARAMETER, Memory::DOSERROR);
		this->ID = id;
		this->special = true;
	}
}