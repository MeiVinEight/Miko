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

	WSA_API extern const DWORD ERRNO_UNKNOWN_HOST;
	WSA_API extern const DWORD ERRNO_SOCKET_ALREADY_OCCUPIED;

	class Address;
	// IPv4 IP and PORT
	class SocketAddress;
	// Base Socket operation
	class Socket;

	WSA_API WSA::Address IP(const char *);
	WSA_API SOCKET socket(int, int, int);
	WSA_API void bind(SOCKET, void *, int);
	WSA_API void listen(SOCKET, int);
	WSA_API void connect(SOCKET, const void *, int);
	WSA_API SOCKET accept(SOCKET, const void *, int *);

	class Address
	{
		public:
		BYTE address[16]{0};

		WSA_API Address();
		WSA_API Address(BYTE, BYTE, BYTE, BYTE);
		WSA_API Address(DWORD);
		WSA_API Address(QWORD, QWORD);
		WSA_API DWORD make() const;
		WSA_API void take(DWORD);
		WSA_API Memory::string string(bool) const;
		WSA_API Memory::string string() const;
		WSA_API bool IPV4() const;
	};
	class SocketAddress
	{
		public:
		WSA::Address IP;
		WORD ID = 0;

		WSA_API SocketAddress();
		WSA_API SocketAddress(BYTE, BYTE, BYTE, BYTE, WORD);
		WSA_API SocketAddress(DWORD, WORD);
		WSA_API SocketAddress(QWORD, QWORD, WORD);
		WSA_API SocketAddress(WORD);
		WSA_API SocketAddress(const WSA::Address &, WORD);
	};
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
		WSA_API Socket();
		WSA_API Socket(WSA::Socket &&) noexcept;
		WSA_API WSA::Socket &operator=(WSA::Socket &&) noexcept;
		WSA_API void socket();
		WSA_API void option(int, int, void *, int *, BYTE) const;
		WSA_API void bind(const WSA::SocketAddress &);
		WSA_API void listen() const;
		WSA_API void connect(WSA::SocketAddress addr);
		WSA_API WSA::Socket accept() const;
		WSA_API DWORD read(void *, DWORD) override;
		WSA_API DWORD write(const void *, DWORD) override;
		WSA_API void flush() override;
		WSA_API QWORD available() override;
		WSA_API bool select(int, long, long) const;
		WSA_API bool select(int) const;
		WSA_API void shutdown(int) const;
		WSA_API BOOL opening() const;
		WSA_API void close();
	};
}

#endif //WSA_WSA_H