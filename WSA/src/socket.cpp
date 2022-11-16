#include "wsadef.h"

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
			throw Exception::exception(Exception::message(err));
		}
		this->IP = addr.IP;
		this->RP = addr.ID;

		int len = sizeof(SOCKADDR_IN);
		getsockname(this->connection, (SOCKADDR *)&addr_in, &len);
		this->LP = htons(addr_in.sin_port);

		return;
	}
	throw Exception::exception("Already connected");
}

DWORD WSA::Socket::read(void *b, DWORD len)
{
	if (~this->connection)
	{
		DWORD readed = recv(this->connection, (char *)b, (int)len, 0);
		if (readed == (DWORD)SOCKET_ERROR)
		{
			DWORD err = WSAGetLastError();
			if (err != WSAECONNABORTED)
			{
				throw Exception::exception(Exception::message(err));
			}
			this->close();
			throw Exception::exception("Socket closed");
		}
		return readed;
	}
	throw Exception::exception("Socket closed");
}

DWORD WSA::Socket::write(void *b, DWORD len)
{
	if (~this->connection)
	{
		DWORD sended = send(this->connection, (char *)b, (int)len, 0);
		if (sended == SOCKET_ERROR)
		{
			DWORD err = WSAGetLastError();
			if (err != WSAECONNABORTED)
			{
				throw Exception::exception(Exception::message(err));
			}
			this->close();
			throw Exception::exception("Socket closed");
		}
		return sended;
	}
	throw Exception::exception("Socket closed");
}

QWORD WSA::Socket::available()
{
	u_long ava = 0;
	if (~ioctlsocket(this->connection, FIONREAD, &ava))
	{
		return ava;
	}
	throw Exception::exception(Exception::message(WSAGetLastError()));
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
			throw Exception::exception(Exception::message(WSAGetLastError()));
		}
	}
}