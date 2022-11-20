#ifndef WSA_WSA_H
#define WSA_WSA_H

#ifdef WSA_SHARED
	#define WSA_API __declspec(dllexport)
#else
	#define WSA_API
#endif

#include <filesystem.h>

typedef int					BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;

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
	WSA::Address IP(LPCSTR host);
	WSA_API
	SOCKET socket();

	// TODO IPv6
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

		ServerSocket(const WSA::ServerSocket &) = delete;
		WSA::ServerSocket &operator=(const WSA::ServerSocket &) = delete;
		WSA_API
		ServerSocket();
		WSA_API
		ServerSocket(WSA::ServerSocket &&);
		WSA_API
		~ServerSocket();
		WSA_API
		WSA::ServerSocket &operator=(WSA::ServerSocket &&);
		WSA_API
		void bind(const WSA::SocketAddress &);
		WSA_API
		WSA::Socket accept() const;
		WSA_API
		BOOL opening() const;
		WSA_API
		void close();
	};

	class Socket: public FileSystem::AbstractStream
	{
		public:
		SOCKET connection = INVALID_SOCKET; // INVALID_SOCKET
		WSA::Address IP;
		WORD RP = 0; // remote port
		WORD LP = 0; // local port

		Socket(const WSA::Socket &) = delete;
		WSA::Socket &operator=(const WSA::Socket &) = delete;
		WSA_API
		Socket();
		WSA_API
		Socket(WSA::Socket &&);
		WSA_API
		WSA::Socket &operator=(WSA::Socket &&);
		WSA_API
		void connect(WSA::SocketAddress addr);
		WSA_API
		void read(void *, DWORD) override;
		WSA_API
		void write(const void *, DWORD) override;
		WSA_API
		QWORD available() override;
		WSA_API
		BOOL opening() const;
		WSA_API
		void close();
	};
}

#endif //WSA_WSA_H
