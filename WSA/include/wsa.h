#ifndef WSA_WSA_H
#define WSA_WSA_H

#include <WSASpec.h>
#include <WinType.h>

typedef unsigned long long SOCKET;

namespace WSA
{
	static const SOCKET INVALID_SOCKET = ~((SOCKET) 0);

	static const int AF_UNSPEC = 0;
	static const int AF_INET   = 2;
	static const int AF_INET6  = 23;

	static const int ST_STREAM = 1;

	static const int PROTO_IP_TCP = 6;

	static const BYTE OPT_GET = 0;
	static const BYTE OPT_SET = 1;

	static const int OPTLVL_IPPROTO_IPV6 = 41;

	static const int OPT_IPV6_V6ONLY = 27;

	static const int SOCK_RD = 0;
	static const int SOCK_WT = 1;
	static const int SOCK_RW = 2;

	WSAAPI extern const DWORD ERRNO_UNKNOWN_HOST;
	WSAAPI extern const DWORD ERRNO_SOCKET_ALREADY_OCCUPIED;

	WSAAPI SOCKET socket(int, int, int);
	WSAAPI void bind(SOCKET, void *, int);
	WSAAPI void listen(SOCKET, int);
	WSAAPI void connect(SOCKET, const void *, int);
	WSAAPI SOCKET accept(SOCKET, const void *, int *);
	WSAAPI int receive(SOCKET, void *, int, int);
	WSAAPI int send(SOCKET, const void *, int, int);
	WSAAPI int close(SOCKET);
	WSAAPI int select(int, void *, void *, void *, const void *);
}

#endif //WSA_WSA_H