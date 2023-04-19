#include "definitions.h"

CWS::WebSocket::WebSocket(): random(new Cryptography::MersenneTwister)
{
	this->random->seed(Timestamp::current());
}
CWS::WebSocket::WebSocket(const CWS::WebSocket &copy)
{
	this->connection = copy.connection;
	this->random = new Cryptography::MersenneTwister;
	this->random->seed(Timestamp::current());
	this->opening = copy.opening;
	this->control = copy.control;
}
CWS::WebSocket::WebSocket(CWS::WebSocket &&move) noexcept: control((CWS::Message &&) move.control)
{
	this->connection = move.connection;
	this->random = move.random;
	this->opening = move.opening;
	move.random = nullptr;
}
CWS::WebSocket::~WebSocket()
{
	delete this->random;
}
CWS::WebSocket &CWS::WebSocket::operator=(const CWS::WebSocket &copy)
{
	if (this != &copy)
	{
		(*this) = CWS::WebSocket(copy);
	}
	return *this;
}
CWS::WebSocket &CWS::WebSocket::operator=(CWS::WebSocket &&move) noexcept
{
	if (this != &move)
	{
		this->connection = move.connection;
		this->random = move.random;
		this->random = move.random;
		this->opening = move.opening;
		move.random = nullptr;
	}
	return *this;
}
bool CWS::WebSocket::alive() const
{
	return this->opening;
}
CWS::Message CWS::WebSocket::frame() const
{
	CWS::Message msg;
	Streaming::fully conn(this->connection);
	BYTE buf[16];
	conn.read(buf, 1);
	msg.FIN = (buf[0] >> 7) & 0x1;
	msg.RSV = (buf[0] >> 4) & 0x7;
	msg.OPC = (buf[0] >> 0) & 0xF;

	conn.read(buf, 1);
	BYTE MASK = (buf[0] >> 7) & 1;

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
	msg.context.resize(length);
	conn.read(msg.context.address, msg.context.length);
	if (MASK)
	{
		for (QWORD i = 0; i < msg.context.length; i++)
		{
			msg.context[i] ^= maskingKey[i % 4];
		}
	}
	return msg;
}
void CWS::WebSocket::frame(const CWS::Message &frame) const
{
	if (!this->alive())
		throw Memory::exception(Memory::ERRNO_OBJECT_CLOSED);

	const Memory::string &payload = frame.context;
	Streaming::fully conn(this->connection);
	BYTE prefix[14] = {0};

	prefix[0] |= (frame.FIN & 0x1) << 7;
	prefix[0] |= (frame.RSV & 0x7) << 4;
	prefix[0] |= (frame.OPC & 0xF) << 0;

	// MSK = 1
	prefix[1] |= (frame.MSK & 0x1) << 7;
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
	conn.write(prefix, offset);

	DWORD mask = 0;
	if (frame.MSK)
	{
		mask = this->random->random();
		conn.write(&mask, 4);
	}
	BYTE *maskingKey = (BYTE *) &mask;

	Memory::string data(payload.length);
	Memory::copy(data.address, payload.address, payload.length);
	for (QWORD i = 0; i < data.length; i++)
	{
		data[i] ^= maskingKey[i % 4];
	}
	conn.write(data.address, data.length);
	this->connection->flush();
}
CWS::Message CWS::WebSocket::receive()
{
	CWS::Message msg;
	Memory::string &payload = msg.context;
	BYTE FIN = 0;
	BYTE RSV = 0xFF;
	BYTE OPC = 0xFF;
	while (!FIN)
	{
		CWS::Message frame = this->frame();
		if (frame.OPC & 0x80)
		{
			if (frame.OPC == CWS::OPC_PING)
			{
				CWS::Message pong = frame;
				pong.OPC = CWS::OPC_PONG;
				this->transmit(pong);
			}
			this->control = frame;
		}
		else
		{
			FIN = frame.FIN;
			RSV = (RSV == 0xFF) ? frame.RSV : RSV;
			OPC = (OPC == 0xFF) ? frame.OPC : OPC;

			QWORD offset = payload.length;
			payload.resize(payload.length + frame.context.length);
			Memory::copy(payload.address + offset, frame.context.address, frame.context.length);
		}
	}
	msg.RSV = RSV;
	msg.OPC = OPC;
	return msg;
}
void CWS::WebSocket::transmit(const CWS::Message &msg)
{
	this->frame(msg);
	if (msg.OPC == CWS::OPC_CLOSE)
		this->opening = false;
}