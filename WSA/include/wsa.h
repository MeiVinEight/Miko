#ifndef WSA_WSA_H
#define WSA_WSA_H

#ifdef WSA_SHARED
	#define WSA_API __declspec(dllexport)
#else
	#define WSA_API __declspec(dllimport)
#endif

#include <streaming.h>

typedef unsigned __int64 SOCKET;

namespace WSA
{
	static const SOCKET INVALID_SOCKET = ~((SOCKET) 0);
	WSA_API extern const DWORD ERRNO_UNKNOWN_HOST;
	WSA_API extern const DWORD ERRNO_SOCKET_ALREADY_OCCUPIED;

	class Address;
	// IPv4 IP and PORT
	class SocketAddress;
	// Base ServerSocket operation
	class ServerSocket;
	class Socket;

	WSA_API WSA::Address IP(const char *);
	WSA_API SOCKET socket();

	// TODO IPv6
	class Address
	{
		public:
		BYTE address[4]{0};

		WSA_API DWORD make() const;
		WSA_API void take(DWORD);
		WSA_API Memory::string string() const;
	};
	class SocketAddress
	{
		public:
		WSA::Address IP;
		WORD ID = 0;
	};
	class Socket: public Streaming::stream
	{
		public:
		SOCKET connection = WSA::INVALID_SOCKET; // INVALID_SOCKET
		int suspend = 0x7FFFFFFF;
		WSA::Address IP;
		WORD RP = 0; // remote port
		WORD LP = 0; // local port

		Socket(const WSA::Socket &) = delete;
		WSA::Socket &operator=(const WSA::Socket &) = delete;
		WSA_API Socket();
		WSA_API Socket(WSA::Socket &&) noexcept;
		WSA_API WSA::Socket &operator=(WSA::Socket &&) noexcept;
		WSA_API void bind(const WSA::SocketAddress &);
		WSA_API WSA::Socket accept() const;
		WSA_API void connect(WSA::SocketAddress addr);
		WSA_API DWORD read(void *, DWORD) override;
		WSA_API DWORD write(const void *, DWORD) override;
		WSA_API void flush() override;
		WSA_API QWORD available() override;
		WSA_API BOOL opening() const;
		WSA_API void close();
	};
}

#endif //WSA_WSA_H