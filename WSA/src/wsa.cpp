#include "wsadef.h"

void WSA::startup()
{
	WSADATA data;
	int err = WSAStartup(0x0202, &data);
	if (err)
	{
		throw Exception::exception(Exception::exception::INTERNAL, err);
	}
}

void WSA::cleanup()
{
	if (WSACleanup() == SOCKET_ERROR)
	{
		throw Exception::exception(Exception::exception::INTERNAL, WSAGetLastError());
	}
}

WSA::Address WSA::IP(LPCSTR host)
{
	WSA::Address address;
	HOSTENT *ho = gethostbyname(host);
	if (ho == NULL)
	{
		DWORD err = WSAGetLastError();
		if (err)
		{
			throw Exception::exception(Exception::exception::INTERNAL, err);
		}
	}
	IN_ADDR *addr = (IN_ADDR*)*ho->h_addr_list;
	address.address[0] = addr->S_un.S_un_b.s_b1;
	address.address[1] = addr->S_un.S_un_b.s_b2;
	address.address[2] = addr->S_un.S_un_b.s_b3;
	address.address[3] = addr->S_un.S_un_b.s_b4;
	return address;
}

SOCKET WSA::socket()
{
	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (~sock)
	{
		return sock;
	}
	throw Exception::exception(Exception::exception::INTERNAL, WSAGetLastError());
}