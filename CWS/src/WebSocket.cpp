#include "definitions.h"

CWS::WebSocket::WebSocket(WSA::Socket *conn): connection(conn), random(new Cryptography::MersenneTwister)
{
	this->random->seed(Timestamp::current());
}
CWS::WebSocket::WebSocket(CWS::WebSocket &&move) noexcept = default;
CWS::WebSocket::~WebSocket()
{
	delete this->random;
}
CWS::WebSocket &CWS::WebSocket::operator=(CWS::WebSocket &&move) noexcept = default;
bool CWS::WebSocket::alive() const
{
	return this->opening;
}
CWS::Message CWS::WebSocket::receive() const
{
	CWS::Message msg;
	Streaming::fully conn(this->connection);
	Memory::string &payload = msg.context;
	BYTE buf[16];
	BYTE FIN = 0;
	BYTE rsv = 0xFF;
	BYTE opc = 0xFF;
	while (!FIN)
	{
		conn.read(buf, 1);
		FIN = (buf[0] >> 7) & 0x1;
		BYTE RSV = (buf[0] >> 4) & 0x7;
		BYTE OPC = buf[0] & 0xF; // TODO process opcode
		rsv = (rsv == 0xFF) ? RSV : rsv;
		opc = (opc == 0xFF) ? OPC : opc;

		conn.read(buf, 1);
		BYTE MASK = (buf[0] >> 7) & 0x1;

		QWORD length = buf[0] & 0x7F;
		if (length == 126)
		{
			conn.read(buf, 2);
			length = (buf[0] << 8) | buf[1];
		}
		else if (length == 127)
		{
			conn.read(buf, 8);
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
			conn.read(maskingKey, 4);
		}

		QWORD offset = payload.length;
		payload.resize(payload.length + length);
		conn.read(payload.address + offset, length);
		if (MASK)
		{
			for (QWORD i = 0; i < offset; i++)
			{
				payload[offset + i] ^= maskingKey[i % 4];
			}
		}
	}
	msg.RSV = rsv;
	msg.OPC = opc;
	return msg;
}
void CWS::WebSocket::send(const CWS::Message &msg) const
{
	const Memory::string &payload = msg.context;
	Streaming::fully conn(this->connection);
	BYTE prefix[14];

	prefix[0] = 0x82;
	prefix[0] |= (msg.RSV & 0x7) << 4;
	prefix[0] |= (msg.OPC & 0xF);

	// MSK = 1
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
	DWORD mask = this->random->random();
	BYTE *maskingKey = (BYTE *) &mask;
	Memory::copy(prefix + offset, &mask, 4);

	Memory::string data(payload.length);
	Memory::copy(data.address, payload.address, payload.length);
	for (QWORD i = 0; i < data.length; i++)
	{
		data[i] ^= maskingKey[i % 4];
	}
	conn.write(prefix, offset + 4);
	conn.write(data.address, data.length);
	this->connection->flush();
}
void CWS::WebSocket::close()
{
	this->opening = false;
}