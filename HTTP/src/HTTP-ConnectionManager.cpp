#include "httpdef.h"

HTTP::ConnectionManager::ConnectionManager(const WSA::SocketAddress &address)
{
	this->connection.connect(address);
}

HTTP::ConnectionManager::ConnectionManager(HTTP::ConnectionManager &&move): connection((WSA::Socket &&)move)
{
}

HTTP::ConnectionManager::~ConnectionManager()
{
	this->connection.close();
}

HTTP::ConnectionManager &HTTP::ConnectionManager::operator=(HTTP::ConnectionManager &&move)
{
	if (&move != this)
	{
		this->connection = (WSA::Socket &&) move.connection;
	}
	return *this;
}

void HTTP::ConnectionManager::send(const HTTP::Message &msg)
{
	String::string rm = HTTP::method(msg.method);
	this->connection.write(rm.address, rm.length);
	char buf[11];
	buf[0] = ' ';
	this->connection.write(buf, 1);
	this->connection.write(msg.URL.address, msg.URL.length);
	Memory::copy(buf, " HTTP/", 6);
	buf[6] = (char)('0' + ((msg.version >> 8) & 0xFF));
	buf[7] = '.';
	buf[8] = (char)('0' + ((msg.version >> 0) & 0xFF));
	buf[9] = '\r';
	buf[10] = '\n';
	this->connection.write(buf, 11);

	buf[0] = ':';
	buf[1]=  ' ';
	buf[2] = '\r';
	buf[3] = '\n';
	for (QWORD i = 0; i < msg.length; i++)
	{
		String::string &key = msg.context[i][0];
		String::string &val = msg.context[i][1];
		this->connection.write(key.address, key.length);
		this->connection.write(buf, 2);
		this->connection.write(val.address, val.length);
		this->connection.write(buf + 2, 2);
	}
	this->connection.write(buf + 2, 2);
	this->connection.write(msg.content, msg.content.length);
}

HTTP::Message HTTP::ConnectionManager::accept()
{
	HTTP::Message message;
	char buf[64];
	this->connection.read(buf, 13);
	WORD majVer = buf[5] - '0';
	WORD minVer = buf[7] - '0';
	message.version = (majVer << 8) | minVer;
	message.status = 100 * (buf[9] - '0') + 10 * (buf[10] - '0') + (buf[11] - '0');
	buf[0] = 0;
	QWORD idx = 0;
	while (true)
	{
		char ch;
		this->connection.read(&ch, 1);
		if (ch != '\n')
		{
			if (ch != '\r')
			{
				buf[idx++] = ch;
				buf[idx] = 0;
			}
			continue;
		}
		break;
	}
	buf[idx] = 0;
	// HTTP header
	Memory::string str[2] = {Memory::string(65), Memory::string(65)};
	str[0][0] = str[1][0] = 0;
	WORD type = 0;
	QWORD len = 0;
	idx = 0;
	while (true)
	{
		char ch;
		this->connection.read(&ch, 1);
		len += (ch != '\r' && ch != '\n');
		if (ch == '\n')
		{
			if (!len)
			{
				break;
			}
			len = 0;
			idx = 0;
			type = 0;
			message[(void *)str[0]] = str[1];
		}
		else if (ch == ':')
		{
			type++;
			this->connection.read(&ch, 1);
			idx = 0;
		}
		else if (ch != '\r')
		{
			if (idx >= str[type].length)
			{
				str[type].resize(str[type].length + 64);
			}
			str[type][idx++] = ch;
			str[type][idx] = 0;
		}
	}
	if (message.contain("Content-Length"))
	{
		QWORD contentLength = 0;
		String::string &cl = message["Content-Length"];
		for (QWORD i = 0; i < cl.length; i++)
		{
			contentLength *= 10;
			contentLength += cl[i] - '0';
		}
		message.content.resize(contentLength);
		this->connection.read(message.content, contentLength);
	}
	QWORD ava = this->connection.available();
	if (ava && message["Connection"] == "close")
	{
		message.content.resize(ava);
		this->connection.read(message.content, ava);
	}
	return message;
}

void HTTP::ConnectionManager::close()
{
	this->connection.close();
}