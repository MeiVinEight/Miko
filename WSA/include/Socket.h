#ifndef SOCKET_H
#define SOCKET_H

#include <WinType.h>
#include <streaming.h>
#include <WSASpec.h>
#include <wsa.h>
#include <Address.h>
#include <SocketAddress.h>

namespace WSA
{
	class Socket: public Streaming::stream
	{
		public:
		SOCKET connection = WSA::INVALID_SOCKET; // INVALID_SOCKET
		WORD family = WSA::AF_INET6;
		WORD type = WSA::ST_STREAM;
		WORD protocol = 0;
		WSA::Address IP;
		WORD RP = 0; // remote port
		WORD LP = 0; // local port
		int suspend = 0x7FFFFFFF;

		Socket(const WSA::Socket &) = delete;
		WSA::Socket &operator=(const WSA::Socket &) = delete;
		WSAAPI Socket();
		WSAAPI Socket(WSA::Socket &&) noexcept;
		WSAAPI WSA::Socket &operator=(WSA::Socket &&) noexcept;
		WSAAPI void socket();
		WSAAPI void option(int, int, void *, int *, BYTE) const;
		WSAAPI void bind(const WSA::SocketAddress &);
		WSAAPI void listen() const;
		WSAAPI void connect(WSA::SocketAddress addr);
		WSAAPI WSA::Socket accept() const;
		WSAAPI DWORD read(void *, DWORD) override;
		WSAAPI DWORD write(const void *, DWORD) override;
		WSAAPI void flush() override;
		WSAAPI QWORD available() override;
		WSAAPI bool select(int, long, long) const;
		WSAAPI bool select(int) const;
		WSAAPI void shutdown(int) const;
		WSAAPI BOOL opening() const;
		WSAAPI void close();
	};
}

#endif //SOCKET_H
