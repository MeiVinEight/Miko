#ifndef WSA_WSA_H
#define WSA_WSA_H

#include "wsadef.h"
#include "fs.h"

#define INVALID_SOCKET  (SOCKET)(~0)

typedef const char *LPCSTR;
typedef unsigned __int64 SOCKET;

namespace WSA
{
	const static WORD SOCKET_CLOSED = 1;
	const static WORD SOCKET_BOUND  = 2;

	class Address;
	// IPv4 IP and PORT
	class SocketAddress;
	// Base ServerSocket operation
	class ServerSocket;
	class Socket;

	WSA_API
	void startup();
	WSA_API
	void cleanup() noexcept(false);
	WSA_API
	WSA::Address IP(LPCSTR host);
	WSA_API
	SOCKET socket();

	class Address
	{
		public:
		BYTE address[4]{0};

		WSA_API
		DWORD make() const;
		WSA_API
		void take(DWORD);
	};
	class SocketAddress
	{
		public:
		WSA::Address IP;
		WORD ID = 0;
	};

	class ServerSocket
	{
		public:
		// System network handle.
		SOCKET connection = INVALID_SOCKET; // INVALID_SOCKET
		WORD backlog = 50;
		WSA::SocketAddress address;

		WSA_API
		ServerSocket() = default;
		WSA_API
		void bind(const WSA::SocketAddress &);
		WSA_API
		WSA::Socket accept() const;
		WSA_API
		BOOL opening() const;
		WSA_API
		void close();
	};

	class Socket: public WSA::BIO
	{
		public:
		SOCKET connection = INVALID_SOCKET; // INVALID_SOCKET
		WSA::Address IP;
		WORD RP{}; // remote port
		WORD LP{}; // local port

		WSA_API
		Socket();
		WSA_API
		void connect(WSA::SocketAddress addr);
		WSA_API
		DWORD read(BYTE *, DWORD) override;
		WSA_API
		DWORD write(BYTE *, DWORD) override;
		WSA_API
		QWORD available() override;
		WSA_API
		BOOL opening() const;
		WSA_API
		void close();
	};
}

#endif //WSA_WSA_H
