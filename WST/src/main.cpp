#include <iostream>

#include <wsa.h>
#include <exception.h>
#include <sstring.h>

int main()
{
	try
	{
		WSA::startup();

		WSA::Socket socket;
		socket.connect({WSA::IP("broadcastlv.chat.bilibili.com"), 2244});
		String::string s = "GET /sub HTTP/1.0\r\n"
						   "Host: broadcastlv.chat.bilibili.com\r\n"
						   "Upgrade: websocket\r\n"
						   "Connection: Upgrade\r\n"
						   "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
						   "Sec-WebSocket-Protocol: chat, superchat\r\n"
						   "Sec-WebSocket-Version: 13\r\n"
						   "Origin: ws://broadcastlv.chat.bilibili.com:2244/sub\r\n"
						   "\r\n";
		socket.write((BYTE *) s.address.address, s.length);
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
	catch (WSA::exception &exec)
	{
		std::cout << "Error: " << exec.value << std::endl;
		for (DWORD i = 0; i < exec.count; i++)
		{
			WSA::exception::frame &frame = exec.stack[i];
			std::cout	<< "0x"
						<< frame.offset
						<< " ("
						<< frame.function
						<< "+0x"
						<< std::hex
						<< ((QWORD)frame.offset - (QWORD)frame.address)
						<< std::dec
						<< ") ["
						<< frame.library
						<< "+0x"
						<< std::hex
						<< ((QWORD)frame.offset - (QWORD)frame.module)
						<< std::dec
						<< "] <";
			if (frame.source)
			{
				std::cout << frame.source << ':' << frame.line;
			}
			else
			{
				std::cout << "Unknown";
			}
			std::cout << '>' << std::endl;
		}
	}
}
