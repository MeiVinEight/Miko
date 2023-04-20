#include "definitions.h"

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
void WSA::Socket::bind(const WSA::SocketAddress &endpoint)
{
	if (this->connection == WSA::INVALID_SOCKET)
	{
		this->IP = endpoint.IP;
		this->LP = endpoint.ID;

		// Create an unbound socket
		this->connection = WSA::socket();

		// bind this socket to address
		SOCKADDR_IN addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = htonl(endpoint.IP.make());
		addr.sin_port = htons(endpoint.ID);
		int err = ::bind(this->connection, (SOCKADDR *) &addr, sizeof(SOCKADDR_IN));
		err = err ? err : listen(this->connection, this->suspend);
		if (err)
		{
			err = WSAGetLastError();
			this->close();
			throw Memory::exception(err, Memory::DOSERROR);
		}
		return;
	}
	throw Memory::exception(WSA::ERRNO_SOCKET_ALREADY_OCCUPIED);
}
WSA::Socket WSA::Socket::accept() const
{
	if (~this->connection)
	{
		SOCKADDR_IN addr;
		int len = sizeof(SOCKADDR_IN);

		SOCKET conn = ::accept(this->connection, (SOCKADDR *) &addr, &len);
		if (conn == WSA::INVALID_SOCKET)
		{
			throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
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
	throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);
}
void WSA::Socket::connect(WSA::SocketAddress addr)
{
	if (this->connection == WSA::INVALID_SOCKET)
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
			throw Memory::exception(err, Memory::DOSERROR);
		}
		this->IP = addr.IP;
		this->RP = addr.ID;

		int len = sizeof(SOCKADDR_IN);
		getsockname(this->connection, (SOCKADDR *)&addr_in, &len);
		this->LP = htons(addr_in.sin_port);

		return;
	}
	throw Memory::exception(WSA::ERRNO_SOCKET_ALREADY_OCCUPIED);
}
DWORD WSA::Socket::read(void *b, DWORD len)
{
	if (~this->connection)
	{
		DWORD readed = recv(this->connection, (char *) b, (int) len, 0);
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
		DWORD sended = send(this->connection, (const char *) b, (int) len, 0);
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
	u_long ava = 0;
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
	fd_set rd = {};
	fd_set wt = {};
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
			throw Memory::exception(Memory::ERRNO_INVALID_PARAMETER);
		}
	}
	timeval *wait = nullptr;
	if (s != -1)
		wait = &val;

	int result = ::select(0, &rd, &wt, nullptr, wait);
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
		int wrong = closesocket(this->connection);
		this->connection = WSA::INVALID_SOCKET;
		this->IP = {0, 0, 0, 0};
		this->RP = this->LP = 0;
		if (wrong)
		{
			throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
		}
	}
}