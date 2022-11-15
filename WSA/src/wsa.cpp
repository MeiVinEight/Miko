#pragma comment(lib, "ws2_32.lib") // import ws2_32.lib with 2s

#define WSA_SHARED
#include <wsa.h>
#include <exception.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define NULL				0
#define PASCAL				__stdcall
#define WSAAPI				PASCAL
#define WSADESCRIPTION_LEN	256
#define WSASYS_STATUS_LEN	128
#define SOCKET_ERROR		(-1)
#define AF_INET				2               /* internetwork: UDP, TCP, etc. */
#define SOCK_STREAM			1               /* stream socket */
#define IPPROTO_TCP			6               /* tcp */
#define WSAECONNABORTED		10053L
#define FIONREAD			(0x40000000 | (((long) sizeof(u_long) & 0x7f) << 16) | (('f') << 8) | (127))

typedef unsigned long	u_long;
typedef unsigned short	u_short;
typedef BYTE 			UCHAR;
typedef unsigned short	USHORT;
typedef DWORD			ULONG;
typedef struct
{
	WORD					wVersion;
	WORD					wHighVersion;
	unsigned short			iMaxSockets;
	unsigned short			iMaxUdpDg;
	char *					lpVendorInfo;
	char					szDescription[WSADESCRIPTION_LEN+1];
	char					szSystemStatus[WSASYS_STATUS_LEN+1];
} WSADATA, *LPWSADATA;
typedef struct
{
	char    * h_name;           /* official name of host */
	char    ** h_aliases;  /* alias list */
	short   h_addrtype;             /* host address type */
	short   h_length;               /* length of address */
	char    ** h_addr_list; /* list of addresses */
} HOSTENT;
typedef struct
{
	union
	{
		struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { USHORT s_w1,s_w2; } S_un_w;
		ULONG S_addr;
	} S_un;
} IN_ADDR;
typedef struct
{
	short   sin_family;
	u_short sin_port;
	IN_ADDR sin_addr;
	char    sin_zero[8];
} SOCKADDR_IN;
typedef struct
{
	u_short sa_family;              /* address family */
	char    sa_data[14];            /* up to 14 bytes of direct address */
} SOCKADDR;

int WSAAPI WSAStartup(WORD, LPWSADATA);
int WSAAPI WSACleanup(void);
int WSAAPI WSAGetLastError(void);
HOSTENT *WSAAPI gethostbyname(const char *);
SOCKET WSAAPI socket(int, int, int);
u_long WSAAPI htonl(u_long);
u_short WSAAPI htons(u_short);
int WSAAPI bind(SOCKET, const SOCKADDR*, int);
int WSAAPI listen(SOCKET, int);
SOCKET WSAAPI accept(SOCKET, SOCKADDR*, int *);
int WSAAPI closesocket(SOCKET);
int WSAAPI connect(SOCKET, const SOCKADDR *, int);
int WSAAPI recv(SOCKET, char *, int, int);
int WSAAPI send(SOCKET, const char *, int, int);
int WSAAPI ioctlsocket(SOCKET, long, u_long *);
int WSAAPI getsockname(SOCKET, SOCKADDR *, int *);


#ifdef __cplusplus
}
#endif

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

DWORD WSA::Address::make() const
{
	return (this->address[0] << 24) | (this->address[1] << 16) | (this->address[2] << 8) | (this->address[3]);
}

void WSA::Address::take(DWORD addr)
{
	this->address[0] = (addr >> 24) & 0xFF;
	this->address[1] = (addr >> 16) & 0xFF;
	this->address[2] = (addr >> 8) & 0xFF;
	this->address[3] = addr & 0xFF;
}

void WSA::ServerSocket::bind(const WSA::SocketAddress &endpoint)
{
	if (this->connection == INVALID_SOCKET)
	{
		this->address = endpoint;

		// Create an unbound socket
		this->connection = WSA::socket();

		// bind this socket to address
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = htonl(endpoint.IP.make());
		addr.sin_port = htons(endpoint.ID);
		int err = ::bind(this->connection, (SOCKADDR *) &addr, sizeof(SOCKADDR_IN));
		err = err ? err : listen(this->connection, this->backlog);
		if (err)
		{
			err = WSAGetLastError();
			this->close();
			throw Exception::exception(Exception::exception::INTERNAL, err);
		}
		return;
	}
	throw Exception::exception(Exception::exception::EXTERNAL, WSA::SOCKET_BOUND);
}

WSA::Socket WSA::ServerSocket::accept() const
{
	if (~this->connection)
	{
		SOCKADDR_IN addr;
		int len = sizeof(SOCKADDR_IN);

		SOCKET conn = ::accept(this->connection, (SOCKADDR *) &addr, &len);
		if (conn == INVALID_SOCKET)
		{
			throw Exception::exception(Exception::exception::EXTERNAL, WSA::SOCKET_CLOSED);
		}
		WSA::Socket sock;
		sock.connection = conn;
		sock.IP.take(htonl(addr.sin_addr.S_un.S_addr));
		sock.RP = htons(addr.sin_port);

		len = sizeof(SOCKADDR_IN);
		getsockname(conn, (SOCKADDR *) &addr, &len);
		sock.LP = htons(addr.sin_port);

		return sock;
	}
	throw Exception::exception(Exception::exception::EXTERNAL, WSA::SOCKET_CLOSED);
}

BOOL WSA::ServerSocket::opening() const
{
	return !!(~this->connection);
}

void WSA::ServerSocket::close()
{
	if (~this->connection)
	{
		if (closesocket(this->connection))
		{
			this->connection = INVALID_SOCKET;
			throw Exception::exception(Exception::exception::INTERNAL, WSAGetLastError());
		}
	}
}

WSA::Socket::Socket():IP(WSA::Address()), RP(0), LP(0)
{
}

void WSA::Socket::connect(WSA::SocketAddress addr)
{
	if (this->connection == INVALID_SOCKET)
	{
		// Create an unbound socket
		this->connection = WSA::socket();

		// connect to server
		SOCKADDR_IN addr_in;
		addr_in.sin_family = AF_INET;
		addr_in.sin_addr.S_un.S_addr = htonl(addr.IP.make());
		addr_in.sin_port = htons(addr.ID);
		if (::connect(this->connection, (SOCKADDR *)&addr_in, sizeof(SOCKADDR_IN)))
		{
			int err = WSAGetLastError();
			this->close();
			throw Exception::exception(Exception::exception::INTERNAL, err);
		}
		this->IP = addr.IP;
		this->RP = addr.ID;

		int len = sizeof(SOCKADDR_IN);
		getsockname(this->connection, (SOCKADDR *)&addr_in, &len);
		this->LP = htons(addr_in.sin_port);

		return;
	}
	throw Exception::exception(Exception::exception::EXTERNAL, WSA::SOCKET_BOUND);
}

DWORD WSA::Socket::read(BYTE *b, DWORD len)
{
	if (~this->connection)
	{
		DWORD readed = recv(this->connection, (char *)b, (int)len, 0);
		if (readed == (DWORD)SOCKET_ERROR)
		{
			throw Exception::exception(Exception::exception::INTERNAL, WSAGetLastError());
		}
		readed ? void() : this->close();
		return readed;
	}
	throw Exception::exception(Exception::exception::EXTERNAL, WSA::SOCKET_CLOSED);
}

DWORD WSA::Socket::write(BYTE *b, DWORD len)
{
	if (~this->connection)
	{
		DWORD sended = send(this->connection, (char *)b, (int)len, 0);
		if (sended == SOCKET_ERROR)
		{
			DWORD err = WSAGetLastError();
			if (err != WSAECONNABORTED)
			{
				throw Exception::exception(Exception::exception::INTERNAL, err);
			}
			this->close();
			throw Exception::exception(Exception::exception::EXTERNAL, WSA::SOCKET_CLOSED);
		}
		return sended;
	}
	throw Exception::exception(Exception::exception::EXTERNAL, WSA::SOCKET_CLOSED);
}

QWORD WSA::Socket::available()
{
	u_long ava = 0;
	if (~ioctlsocket(this->connection, FIONREAD, &ava))
	{
		return ava;
	}
	throw Exception::exception(Exception::exception::INTERNAL, WSAGetLastError());
}

BOOL WSA::Socket::opening() const
{
	return !!(~this->connection);
}

void WSA::Socket::close()
{
	if (~this->connection)
	{
		if (closesocket(this->connection))
		{
			this->connection = INVALID_SOCKET;
			throw Exception::exception(Exception::exception::INTERNAL, WSAGetLastError());
		}
	}
}