#include <iostream>

#include <wsa.h>
#include <exception.h>
#include <sstring.h>
#include <hhttp.h>
#include <filesystem.h>

void wsa()
{
	WSA::startup();

	WSA::Socket socket;
	socket.connect({WSA::IP("broadcastlv.chat.bilibili.com"), 2244});
	String::string s = "GET /sub HTTP/1.1\r\n"
					   "Host: broadcastlv.chat.bilibili.com\r\n"
					   "Upgrade: websocket\r\n"
					   "Connection: Upgrade\r\n"
					   "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
					   "Sec-WebSocket-Protocol: chat, superchat\r\n"
					   "Sec-WebSocket-Version: 13\r\n"
					   "Origin: ws://broadcastlv.chat.bilibili.com:2244/sub\r\n"
					   "\r\n";
	socket.write(s.address.address, s.length);
	int len = 0;
	int cr = 0;
	while (true)
	{
		BYTE byte;
		socket.read(&byte, 1);
		std::cout << byte << std::flush;
		if (byte == '\n' && cr)
		{
			if (len == 1)
			{
				break;
			}
			len = 0;
			continue;
		}
		len ++;
		cr = byte == '\r';
	}
	socket.close();

	WSA::cleanup();
}

void http()
{
	WSA::startup();
	HTTP::ConnectionManager cm({WSA::IP("broadcastlv.chat.bilibili.com"), 2244});
	HTTP::Message msg;
	msg.method = HTTP::RM_GET;
	msg.URL = "/sub";
	msg["Host"] = "broadcastlv.chat.bilibili.com";
	msg["Upgrade"] = "websocket";
	msg["Connection"] = "Upgrade";
	msg["Sec-WebSocket-Key"] = "x3JJHMbDL1EzLkh9GBhXDw==";
	msg["Sec-WebSocket-Protocol"] = "chat, superchat";
	msg["Sec-WebSocket-Version"] = "13";
	msg["Origin"] = "ws://broadcastlv.chat.bilibili.com:2244/sub";
	cm.send(msg);
	msg = cm.accept();
	std::cout << "HTTP/" << (msg.version >> 8) << '.' << (msg.version & 0xFF) << ' ' << msg.status << std::endl;
	for (QWORD i = 0; i < msg.length; i++)
	{
		std::cout << msg[i][0].address.address << ": " << msg[i][1].address.address << std::endl;
	}
	WSA::cleanup();
}

int main()
{
	try
	{
		http();
	}
	catch (Exception::exception &exec)
	{
		std::cout << exec.message.address << std::endl;
		for (DWORD i = 0; i < exec.count; i++)
		{
			Exception::exception::frame &frame = exec.stack[i];
			std::cout	<< "0x"
						<< frame.offset
						<< " ("
						<< frame.function.address
						<< "+0x"
						<< std::hex
						<< ((QWORD)frame.offset - (QWORD)frame.address)
						<< std::dec
						<< ") ["
						<< frame.library.address
						<< "+0x"
						<< std::hex
						<< ((QWORD)frame.offset - (QWORD)frame.module)
						<< std::dec
						<< ']'
						<< std::endl;
		}
	}
}
