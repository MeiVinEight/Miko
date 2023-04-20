#include "definitions.h"

WSA::Socket::Socket() = default;
WSA::Socket::Socket(WSA::Socket &&move) noexcept: connection(move.connection), IP(move.IP), RP(move.RP), LP(move.LP)
{
	move.connection = INVALID_SOCKET;
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
		move.connection = INVALID_SOCKET;
		move.IP = {0, 0, 0, 0};
		move.LP = move.RP = 0;
	}
	return *this;
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
BOOL WSA::Socket::opening() const
{
	return !!(~this->connection);
}
void WSA::Socket::close()
{
	if (~this->connection)
	{
		int wrong = closesocket(this->connection);
		this->connection = INVALID_SOCKET;
		this->IP = {0, 0, 0, 0};
		this->RP = this->LP = 0;
		if (wrong)
		{
			throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
		}
	}
}