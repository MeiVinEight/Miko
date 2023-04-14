#include "definitions.h"

WSA::ServerSocket::ServerSocket() = default;
WSA::ServerSocket::ServerSocket(WSA::ServerSocket &&move) noexcept: connection(move.connection), backlog(move.backlog), address(move.address)
{
	move.connection = INVALID_SOCKET;
	move.address = {{0, 0, 0, 0}, 0};
}
WSA::ServerSocket::~ServerSocket()
{
	this->close();
}
WSA::ServerSocket &WSA::ServerSocket::operator=(WSA::ServerSocket &&move) noexcept
{
	if (&move != this)
	{
		this->close();
		this->connection = move.connection;
		this->backlog = move.backlog;
		this->address = move.address;
		move.connection = INVALID_SOCKET;
		move.address = {{0, 0, 0, 0}, 0};
	}
	return *this;
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
			throw Memory::exception(err, Memory::INTERNAL);
		}
		return;
	}
	throw Memory::exception(WSA::ERRNO_SOCKET_ALREADY_OCCUPIED);
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
BOOL WSA::ServerSocket::opening() const
{
	return !!(~this->connection);
}
void WSA::ServerSocket::close()
{
	if (~this->connection)
	{
		int wrong = closesocket(this->connection);
		this->connection = INVALID_SOCKET;
		this->address = {{0, 0, 0, 0}, 0};
		if (wrong)
		{
			throw Memory::exception(WSAGetLastError(), Memory::INTERNAL);
		}
	}
}