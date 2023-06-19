#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#include <WSASpec.h>
#include <WinType.h>
#include <Address.h>

namespace WSA
{
	// IPv4/IPv6 IP and PORT
	class SocketAddress
	{
		public:
		WSA::Address IP;
		WORD ID = 0;

		WSAAPI SocketAddress();
		WSAAPI SocketAddress(BYTE, BYTE, BYTE, BYTE, WORD);
		WSAAPI SocketAddress(DWORD, WORD);
		WSAAPI SocketAddress(QWORD, QWORD, WORD);
		WSAAPI SocketAddress(WORD);
		WSAAPI SocketAddress(const WSA::Address &, WORD);
	};
}

#endif //SOCKETADDRESS_H
