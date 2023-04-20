#include "definitions.h"

void HTTP::ConnectionManager::transmit(const HTTP::Message &msg) const
{
	Streaming::fully conn(this->connection);
	char buf[11];
	switch (this->bound)
	{
		case HTTP::HTTP_CLIENT:
		{
			Streaming::format format(&conn);
			format << HTTP::method(msg.method) << ' ' << msg.URL << ' ';
			format << "HTTP/";
			format << (char)('0' + ((msg.version >> 8) & 0xFF));
			format << '.';
			format << (char)('0' + ((msg.version >> 0) & 0xFF));
			format << Streaming::CRLF;
			break;
		}
		case HTTP::HTTP_SERVER:
		{
			Streaming::format format(&conn);
			Memory::copy(buf, "HTTP/", 5);
			buf[5] = (char)('0' + ((msg.version >> 8) & 0xFF));
			buf[6] = '.';
			buf[7] = (char)('0' + ((msg.version >> 0) & 0xFF));
			format << buf << ' ' << msg.status << ' ' << HTTP::status(msg.status) << Streaming::CRLF;
			break;
		}
	}

	buf[0] = ':';
	buf[1]=  ' ';
	buf[2] = '\r';
	buf[3] = '\n';
	for (QWORD i = 0; i < msg.length; i++)
	{
		String::string &key = msg.context[i][0];
		String::string &val = msg.context[i][1];
		conn.write(key.address.address, key.length());
		conn.write(buf, 2);
		conn.write(val.address.address, val.length());
		conn.write(buf + 2, 2);
	}
	conn.write(buf + 2, 2);
	conn.write(msg.content.address, msg.content.length);
}
HTTP::Message HTTP::ConnectionManager::receive() const
{
	Streaming::fully conn(this->connection);
	HTTP::Message message;
	char buf[64];
	QWORD idx = 0;
	switch (this->bound)
	{
		case HTTP::HTTP_CLIENT:
		{
			conn.read(buf, 13);
			WORD majVer = buf[5] - '0';
			WORD minVer = buf[7] - '0';
			message.version = (majVer << 8) | minVer;
			message.status = 100 * (buf[9] - '0') + 10 * (buf[10] - '0') + (buf[11] - '0');
			buf[0] = 0;
			while (true)
			{
				char ch;
				conn.read(&ch, 1);
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
			break;
		}
		case HTTP::HTTP_SERVER:
		{
			Streaming::format format(&conn);
			String::string method;
			String::string version;
			format >> method >> message.URL >> version;
			message.method = HTTP::method(method);
			WORD majVer = version[5] - '0';
			WORD minVer = version[7] - '0';
			message.version = (majVer << 8) | minVer;
			format.read(buf, 2);
			break;
		}
	}
	// HTTP header
	Memory::string str[2] = {Memory::string(65), Memory::string(65)};
	str[0][0] = str[1][0] = 0;
	WORD type = 0;
	QWORD len = 0;
	idx = 0;
	while (true)
	{
		char ch;
		conn.read(&ch, 1);
		len += (ch != '\r' && ch != '\n');
		if (ch == '\n')
		{
			if (!len)
			{
				break;
			}
			str[type].resize(idx);
			len = 0;
			idx = 0;
			type = 0;
			message[str[0]] = str[1];
		}
		else if (ch == ':' && type == 0)
		{
			str[type].resize(idx);
			idx = 0;
			type++;
			conn.read(&ch, 1);
		}
		else if (ch != '\r')
		{
			if (idx >= str[type].length)
			{
				str[type].resize(str[type].length + 64);
			}
			str[type][idx++] = ch;
		}
	}
	if (message.contain("Content-Length"))
	{
		QWORD contentLength = 0;
		String::string &cl = message["Content-Length"];
		for (QWORD i = 0; i < cl.length(); i++)
		{
			contentLength *= 10;
			contentLength += cl[i] - '0';
		}
		message.content.resize(contentLength);
		conn.read(message.content.address, contentLength);
	}
	QWORD ava = conn.available();
	if (ava && message["Connection"] == "close")
	{
		message.content.resize(ava);
		conn.read(message.content.address, ava);
	}
	return message;
}