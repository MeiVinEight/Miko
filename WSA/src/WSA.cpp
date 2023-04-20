#include "definitions.h"

const DWORD WSA::ERRNO_UNKNOWN_HOST = Memory::registry("Cannot find target host");
const DWORD WSA::ERRNO_SOCKET_ALREADY_OCCUPIED = Memory::registry("Socket already occupied");

WSA::Address WSA::IP(const char *host)
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
		throw Memory::exception(WSA::ERRNO_UNKNOWN_HOST);
	}
	freeaddrinfo(info);
	throw Memory::exception(err, Memory::DOSERROR);
}
SOCKET WSA::socket()
{
	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (~sock)
	{
		return sock;
	}
	throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
}