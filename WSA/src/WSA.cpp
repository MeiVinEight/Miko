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
			switch (info->ai_family)
			{
				case WSA::AF_INET:
				{
					SOCKADDR_IN *addr = (SOCKADDR_IN *) info->ai_addr;
					IN_ADDR *ia = &addr->sin_addr;
					address.take(htonl(ia->S_un.S_addr));
					break;
				}
				case WSA::AF_INET6:
				{
					SOCKADDR_IN6 *addr = (SOCKADDR_IN6 *) info->ai_addr;
					IN6_ADDR *ia = &addr->sin6_addr;
					Memory::copy(address.address, ia, 16);
					break;
				}
			}
			freeaddrinfo(info);
			return address;
		}
		throw Memory::exception(WSA::ERRNO_UNKNOWN_HOST);
	}
	freeaddrinfo(info);
	throw Memory::exception(err, Memory::DOSERROR);
}
SOCKET WSA::socket(int af, int type, int proto)
{
	SOCKET sock = ::socket(af, type, proto);
	if (~sock)
	{
		return sock;
	}
	throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
}
void WSA::bind(SOCKET sock, void *addr, int size)
{
	if (::bind(sock, (SOCKADDR *) addr, size) == SOCKET_ERROR)
	{
		throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
	}
}
void WSA::listen(SOCKET sock, int back)
{
	if (::listen(sock, back) == SOCKET_ERROR)
	{
		throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
	}
}
void WSA::connect(SOCKET sock, const void *addr, int size)
{
	if (::connect(sock, addr, size) == SOCKET_ERROR)
	{
		throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
	}
}
SOCKET WSA::accept(SOCKET sock, const void *addr, int *size)
{
	SOCKET ret = ::accept(sock, addr, size);
	if (ret == WSA::INVALID_SOCKET)
	{
		throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
	}
	return ret;
}