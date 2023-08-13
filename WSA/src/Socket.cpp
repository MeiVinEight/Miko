#include <winerr.h>
#include <exception.h>
#include <Socket.h>

#include "ws2_32.h"
#include "sockaddr.h"

#define SOCKET_ERROR        (-1)
#define FIONREAD            (0x40000000 | (((long) sizeof(unsigned long) & 0x7f) << 16) | (('f') << 8) | (127))

typedef struct FDSET
{
	unsigned int fd_count;       /* how many are SET? */
	SOCKET       fd_array[64];   /* an array of SOCKETs */
} FDSET;
typedef struct timeval
{
	long tv_sec;         /* seconds */
	long tv_usec;        /* and microseconds */
} TIMEVAL;

extern "C"
{

unsigned short __stdcall htons(unsigned short);
int __stdcall ioctlsocket(SOCKET, long, unsigned long *);
int __stdcall getsockname(SOCKET, void *, int *);
int __stdcall shutdown(SOCKET, int);
int __stdcall setsockopt(SOCKET, int, int, const void *, int);
int __stdcall getsockopt(SOCKET, int, int, void *, int *);

}

WSA::Socket::Socket() = default;
WSA::Socket::Socket(WSA::Socket &&move) noexcept: connection(move.connection), IP(move.IP), RP(move.RP), LP(move.LP)
{
	move.connection = WSA::INVALID_SOCKET;
	move.IP = {0, 0, 0, 0};
	move.LP = move.RP = 0;
}
WSA::Socket &WSA::Socket::operator=(WSA::Socket &&move) noexcept
{
	if (&move != this)
	{
		this->close();
		this->connection = move.connection;
		this->IP = move.IP;
		this->LP = move.LP;
		this->RP = move.RP;
		move.connection = WSA::INVALID_SOCKET;
		move.IP = {0, 0, 0, 0};
		move.LP = move.RP = 0;
	}
	return *this;
}
void WSA::Socket::socket()
{
	if(~this->connection)
		throw Memory::exception(WSA::ERRNO_SOCKET_ALREADY_OCCUPIED);

	this->connection = WSA::socket(this->family, this->type, this->protocol);
	if (this->family == WSA::AF_INET6)
	{
		// Set IPV6_V6ONLY to false, enable dual-stack socket
		DWORD onlyV6 = 0;
		int size = sizeof(onlyV6);
		this->option(WSA::OPTLVL_IPPROTO_IPV6, WSA::OPT_IPV6_V6ONLY, &onlyV6, &size, WSA::OPT_SET);
	}
}
void WSA::Socket::option(int lvl, int opt, void *val, int *len, BYTE req) const
{
	if (~this->connection)
	{
		switch (req)
		{
			case WSA::OPT_GET:
			{
				if (getsockopt(this->connection, lvl, opt, val, len) == SOCKET_ERROR)
				{
					throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
				}
				break;
			}
			case WSA::OPT_SET:
			{
				if (setsockopt(this->connection, lvl, opt, val, *len) == SOCKET_ERROR)
				{
					throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
				}
				break;
			}
		}
		return;
	}
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
void WSA::Socket::bind(const WSA::SocketAddress &endpoint)
{
	if (this->connection == WSA::INVALID_SOCKET)
		this->socket();

	this->IP = endpoint.IP;
	this->LP = endpoint.ID;

	// bind this socket to address
	if (this->family == WSA::AF_INET6)
	{
		SOCKADDR_IN6 addr = {};
		addr.sin6_family = this->family;
		Memory::copy(&addr.sin6_addr, this->IP.address, 16);
		addr.sin6_port = htons(this->LP);
		WSA::bind(this->connection, &addr, sizeof(SOCKADDR_IN6));
	}
	else
	{
		SOCKADDR_IN addr = {};
		addr.sin_family = (short) this->family;
		addr.sin_addr.S_un.S_addr = htonl(this->IP.make());
		addr.sin_port = htons(this->LP);
		WSA::bind(this->connection, &addr, sizeof(SOCKADDR_IN));
	}
}
void WSA::Socket::listen() const
{
	if (this->connection == WSA::INVALID_SOCKET)
		throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
	WSA::listen(this->connection, this->suspend);
}
void WSA::Socket::connect(WSA::SocketAddress addr)
{
	if (this->connection == WSA::INVALID_SOCKET)
		this->socket();

	this->IP = addr.IP;
	this->RP = addr.ID;

	if (this->family == WSA::AF_INET6)
	{
		SOCKADDR_IN6 addr_in = {};
		addr_in.sin6_family = WSA::AF_INET6;
		addr_in.sin6_port = htons(this->RP);
		Memory::copy(addr_in.sin6_addr.u.Byte, this->IP.address, 16);
		WSA::connect(this->connection, &addr_in, sizeof(SOCKADDR_IN6));

		int len = sizeof(SOCKADDR_IN6);
		getsockname(this->connection, &addr_in, &len);
		this->LP = htons(addr_in.sin6_port);
	}
	else
	{
		// connect to server
		SOCKADDR_IN addr_in = {};
		addr_in.sin_family = WSA::AF_INET;
		addr_in.sin_addr.S_un.S_addr = htonl(this->IP.make());
		addr_in.sin_port = htons(this->RP);
		WSA::connect(this->connection, &addr_in, sizeof(SOCKADDR_IN));

		int len = sizeof(SOCKADDR_IN);
		getsockname(this->connection, &addr_in, &len);
		this->LP = htons(addr_in.sin_port);
	}
}
WSA::Socket WSA::Socket::accept() const
{
	if (~this->connection)
	{
		WSA::Socket sock;
		sock.family = this->family;
		sock.type = this->type;
		sock.protocol = this->protocol;
		if (this->family == WSA::AF_INET6)
		{
			SOCKADDR_IN6 addr = {};
			int len = sizeof(SOCKADDR_IN6);

			SOCKET conn = WSA::accept(this->connection, &addr, &len);
			sock.connection = conn;
			Memory::copy(sock.IP.address, addr.sin6_addr.u.Byte, 16);
			sock.RP = htons(addr.sin6_port);

			getsockname(conn, &addr, &len);
			sock.LP = htons(addr.sin6_port);
		}
		else
		{
			SOCKADDR_IN addr;
			int len = sizeof(SOCKADDR_IN);

			SOCKET conn = WSA::accept(this->connection, &addr, &len);
			sock.connection = conn;
			sock.IP.take(htonl(addr.sin_addr.S_un.S_addr));
			sock.RP = htons(addr.sin_port);

			getsockname(conn, &addr, &len);
			sock.LP = htons(addr.sin_port);
		}
		sock.suspend = 0;
		return sock;
	}
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
DWORD WSA::Socket::read(void *b, DWORD len)
{
	if (~this->connection)
	{
		DWORD readed = WSA::receive(this->connection, b, (int) len, 0);
		if (readed)
		{
			if (readed == SOCKET_ERROR)
			{
				DWORD err = WSAGetLastError();
				this->close();
				throw Memory::exception(err, Memory::DOSERROR);
			}
			return readed;
		}
		this->close();
	}
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
DWORD WSA::Socket::write(const void *b, DWORD len)
{
	if (~this->connection)
	{
		DWORD sended = WSA::send(this->connection, b, (int) len, 0);
		if (sended == SOCKET_ERROR)
		{
			DWORD err = WSAGetLastError();
			this->close();
			throw Memory::exception(err, Memory::DOSERROR);
		}
		return sended;
	}
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
void WSA::Socket::flush()
{
}
QWORD WSA::Socket::available()
{
	unsigned long ava = 0;
	if (~ioctlsocket(this->connection, FIONREAD, &ava))
	{
		return ava;
	}
	throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
}
bool WSA::Socket::select(int what, long s, long us) const
{
	TIMEVAL val = {};
	val.tv_sec = s;
	val.tv_usec = us;
	FDSET rd = {};
	FDSET wt = {};
	int expect;
	switch (what)
	{
		case WSA::SOCK_RD:
		{
			expect = 1;
			rd.fd_count = 1;
			rd.fd_array[0] = this->connection;
			break;
		}
		case WSA::SOCK_WT:
		{
			expect = 1;
			wt.fd_count = 1;
			wt.fd_array[0] = this->connection;
			break;
		}
		case WSA::SOCK_RW:
		{
			expect = 2;
			wt.fd_count = rd.fd_count = 1;
			wt.fd_array[0] = rd.fd_array[0] = this->connection;
			break;
		}
		default:
		{
			throw Memory::exception(ERROR_INVALID_PARAMETER, Memory::DOSERROR);
		}
	}
	timeval *wait = nullptr;
	if (s != -1)
		wait = &val;

	int result = WSA::select(0, &rd, &wt, nullptr, wait);
	if (result == SOCKET_ERROR)
	{
		throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
	}
	return result == expect;
}
bool WSA::Socket::select(int what) const
{
	return this->select(what, -1, 0);
}
void WSA::Socket::shutdown(int how) const
{
	if (this->connection == WSA::INVALID_SOCKET)
	{
		throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
	}
	if (::shutdown(this->connection, how) == SOCKET_ERROR)
	{
		throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
	}
}
BOOL WSA::Socket::opening() const
{
	return !!(~this->connection);
}
void WSA::Socket::close()
{
	if (~this->connection)
	{
		int wrong = WSA::close(this->connection);
		this->connection = WSA::INVALID_SOCKET;
		this->IP = {0, 0, 0, 0};
		this->RP = this->LP = 0;
		if (wrong)
		{
			throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
		}
	}
}