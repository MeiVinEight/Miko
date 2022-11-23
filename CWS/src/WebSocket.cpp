#include "definitions.h"

CWS::WebSocket::WebSocket(const WSA::SocketAddress &endpoint, const String::string &URL): manager(endpoint)
{
	HTTP::Message msg;
	msg.method = HTTP::RM_GET;
	msg.URL = URL;

	msg["Host"] = endpoint.IP.string();
	msg["Upgrade"] = "websocket";
	msg["Connection"] = "Upgrade";
	// TODO random b64 and crypto
	msg["Sec-WebSocket-Key"] = "x3JJHMbDL1EzLkh9GBhXDw==";
	msg["Sec-WebSocket-Protocol"] = "chat, superchat";
	msg["Sec-WebSocket-Version"] = "13";
	this->manager.send(msg);
	msg = this->manager.accept();
	if (msg.status != 101) // Switching Protocols
	{
		QWORD cl = msg.content.length;
		throw Exception::exception(cl ? String::string(msg.content, cl).address : HTTP::status(msg.status).address);
	}
}

CWS::WebSocket::WebSocket(CWS::WebSocket &&move): manager((HTTP::ConnectionManager &&)move.manager)
{
}

CWS::WebSocket::~WebSocket()
{
	this->manager.close();
}

CWS::WebSocket &CWS::WebSocket::operator=(CWS::WebSocket &&move)
{
	if (&move != this)
	{
		this->manager.close();
		this->manager = (HTTP::ConnectionManager &&) move.manager;
	}
	return *this;
}

Memory::string CWS::WebSocket::accept()
{
	Memory::string payload(0);
	QWORD offset = 0;
	BYTE buf[16];
	BYTE FIN = 0;
	while (!FIN)
	{
		this->connection.read(buf, 1);
		FIN = (buf[0] >> 7) & 0x1;
		BYTE RSV = (buf[0] >> 4) & 0x7;
		BYTE opcode = buf[0] & 0xF;
		this->connection.read(buf, 1);
		BYTE MASK = (buf[0] >> 7) & 0x1;
		QWORD length = buf[0] & 0x7F;
		if (length == 126)
		{
			this->connection.read(buf, 2);
			length = (buf[0] << 8) | buf[1];
		}
		else if (length == 127)
		{
			this->connection.read(buf, 8);
			length = 0;
			for (WORD i = 0; i < 8; i++)
			{
				length <<= 8;
				length |= buf[i];
			}
		}
		BYTE maskingKey[4]{0};
		if (MASK)
		{
			this->connection.read(maskingKey, 4);
		}
		payload.resize(payload.length + length);
		this->connection.read(payload + offset, length);
		if (MASK)
		{
			for (QWORD i = 0; i < offset; i++)
			{
				payload[offset + i] = (char)(payload[offset + i] ^ maskingKey[i % 4]);
			}
		}
		offset += length;
	}
	return payload;
}

void CWS::WebSocket::send(const Memory::string &payload)
{
	BYTE prefix[14];
	prefix[0] = 0x82;
	prefix[1] = 0x80;
	WORD offset = 2;
	if (payload.length < 126)
	{
		prefix[1] |= payload.length;
	}
	else if (payload.length < 65536)
	{
		offset += 2;
		prefix[1] |= 126;
		prefix[2] = (payload.length >> 0x08) & 0xFF;
		prefix[3] = (payload.length >> 0x00) & 0xFF;
	}
	else
	{
		offset += 8;
		prefix[1] |= 127;
		for (int i = 0; i < 8; i++)
		{
			prefix[2 + i] = (payload.length >> (8 * (7 - i))) & 0xFF;
		}
	}
	// TODO Random MaskingKey
	prefix[offset + 0] = 0;
	prefix[offset + 1] = 0;
	prefix[offset + 2] = 0;
	prefix[offset + 3] = 0;
	this->connection.write(prefix, offset + 4);
	this->connection.write(payload, payload.length);
}

void CWS::WebSocket::close()
{
	this->manager.close();
}