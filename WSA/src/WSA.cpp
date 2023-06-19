#include <exception.h>
#include <wsa.h>

#include "ws2_32.h"
#include "sockaddr.h"

#define SOCKET_ERROR        (-1)

extern "C"
{

SOCKET __stdcall socket(int, int, int);
int __stdcall bind(SOCKET, const void*, int);
int __stdcall listen(SOCKET, int);
SOCKET __stdcall accept(SOCKET, const void *, int *);
int __stdcall closesocket(SOCKET);
int __stdcall connect(SOCKET, const void *, int);
int __stdcall recv(SOCKET, char *, int, int);
int __stdcall send(SOCKET, const char *, int, int);
int __stdcall select(int, void *, void *, void *, const void *);

}

const DWORD WSA::ERRNO_UNKNOWN_HOST = Memory::registry("Cannot find target host");
const DWORD WSA::ERRNO_SOCKET_ALREADY_OCCUPIED = Memory::registry("Socket already occupied");

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
int WSA::close(SOCKET sock)
{
	return closesocket(sock);
}
int WSA::receive(SOCKET sock, void *buf, int len, int flag)
{
	return ::recv(sock, (char *) buf, len, flag);
}
int WSA::send(SOCKET sock, const void *buf, int len, int flag)
{
	return ::send(sock, (const char *) buf, len, flag);
}
int WSA::select(int nfds, void *rd, void *wt, void *rw, const void *timeout)
{
	return ::select(nfds, rd, wt, rw, timeout);
}