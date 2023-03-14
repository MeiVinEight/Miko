#include "definitions.h"

WSA::Address WSA::IP(LPCSTR host)
{
	ADDRINFOA *info = nullptr;
	INT err = getaddrinfo(host, nullptr, nullptr, &info);
	if (!err)
	{
		if (info)
		{
			WSA::Address address;
			SOCKADDR_IN *addr = (SOCKADDR_IN *)info->ai_addr;
			IN_ADDR *ia = &addr->sin_addr;
			address.address[0] = ia->S_un.S_un_b.s_b1;
			address.address[1] = ia->S_un.S_un_b.s_b2;
			address.address[2] = ia->S_un.S_un_b.s_b3;
			address.address[3] = ia->S_un.S_un_b.s_b4;
			freeaddrinfo(info);
			return address;
		}
		char append[] = "Could not find host: ";
		QWORD applen = sizeof(append) - 1;
		QWORD hoslen = strlen(host);
		Memory::string msg(applen + hoslen);
		Memory::copy(msg.address, append, applen);
		Memory::copy(msg.address + applen, host, hoslen);
		throw Exception::exception(msg);
	}
	freeaddrinfo(info);
	throw Exception::exception(Exception::message(err));
}
SOCKET WSA::socket()
{
	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (~sock)
	{
		return sock;
	}
	throw Exception::exception(Exception::message(WSAGetLastError()));
}