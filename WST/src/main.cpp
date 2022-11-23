#include <iostream>

#include <wsa.h>
#include <exception.h>
#include <sstring.h>
#include <hhttp.h>
#include <cws.h>
#include <filesystem.h>
#include <timestamp.h>

void wsa()
{
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
	socket.write(s.address, s.length);
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
}
void http()
{
	HTTP::ConnectionManager cm({WSA::IP("broadcastlv.chat.bilibili.com"), 2244});
	HTTP::Message msg;
	msg.method = HTTP::RM_GET;
	msg.URL = "/sub";
	msg["Host"] = WSA::IP("broadcastlv.chat.bilibili.com").string();
	msg["Upgrade"] = "websocket";
	msg["Connection"] = "Upgrade";
	msg["Sec-WebSocket-Key"] = "x3JJHMbDL1EzLkh9GBhXDw==";
	msg["Sec-WebSocket-Protocol"] = "chat, superchat";
	msg["Sec-WebSocket-Version"] = "13";
	msg["Origin"] = "ws://broadcastlv.chat.bilibili.com:2244/sub";
	cm.send(msg);
	msg = cm.accept();
	std::cout	<< "HTTP/"
				<< (msg.version >> 8)
				<< '.'
				<< (msg.version & 0xFF)
				<< ' '
				<< msg.status
				<< ' '
				<< HTTP::status(msg.status).address
				<< std::endl;
	for (QWORD i = 0; i < msg.length; i++)
	{
		std::cout << msg[i][0].address << ": " << msg[i][1].address << std::endl;
	}
	if (msg.contain("Content-Type"))
	{
		std::cout << std::endl;
		String::string ct = msg["Content-Type"];
		if (ct[0] == 't' && ct[1] == 'e' && ct[2] == 'x' && ct[3] == 't')
		{
			std::cout.write(msg.content, (std::streamsize)msg.content.length);
			std::cout.flush();
		}
	}
}
void ws()
{
	QWORD roomID = 24543023;
	CWS::WebSocket ws({WSA::IP("broadcastlv.chat.bilibili.com"), 2244}, "/sub");

	String::string json = R"({"uid":0,"roomid":24543023,"protover":3,"platform":"web","type":2,"key":""})";
	Memory::string cert(json.length + 16);
	Memory::fill(cert, 0, cert.length);
	cert[0] = (char)((cert.length >> 0x18) & 0xFF);
	cert[1] = (char)((cert.length >> 0x10) & 0xFF);
	cert[2] = (char)((cert.length >> 0x08) & 0xFF);
	cert[3] = (char)((cert.length >> 0x00) & 0xFF);
	cert[5] = 16;
	cert[7] = 1;
	cert[11] = 7;
	cert[15] = 1;
	Memory::copy(cert + 16, json.address, cert.length - 16);
	ws.send(cert);

	Memory::string payload(0);
	while ((payload = ws.accept()))
	{
		Timestamp::calender cal;
		cal.convert();
		Memory::string file(29); //bilibili/YYYY-MM-DD HH-mm-ss
		file[28] = 0;
		Memory::copy(file, "bilibili/", 9);
		WORD data = cal[Timestamp::calender::YEAR];
		for (QWORD i = 4; i--;)
		{
			file[i + 9] = (char) ('0' + (data % 10));
			data /= 10;
		}
		file[13] = '-';
		file[14] = (char) ('0' + (cal[Timestamp::calender::MONTH] / 10));
		file[15] = (char) ('0' + (cal[Timestamp::calender::MONTH] % 10));
		file[16] = '-';
		file[17] = (char) ('0' + (cal[Timestamp::calender::DAY] / 10));
		file[18] = (char) ('0' + (cal[Timestamp::calender::DAY] % 10));
		file[19] = ' ';
		file[20] = (char) ('0' + (cal[Timestamp::calender::HOUR] / 10));
		file[21] = (char) ('0' + (cal[Timestamp::calender::HOUR] % 10));
		file[22] = '-';
		file[23] = (char) ('0' + (cal[Timestamp::calender::MINUTE] / 10));
		file[24] = (char) ('0' + (cal[Timestamp::calender::MINUTE] % 10));
		file[25] = '-';
		file[26] = (char) ('0' + (cal[Timestamp::calender::SECOND] / 10));
		file[27] = (char) ('0' + (cal[Timestamp::calender::SECOND] % 10));
		std::cout << (file + 9) << " -> " << payload.length << std::endl;
		Filesystem::FileStream stream(file);
		stream.write(payload, payload.length);
		stream.close();
	}

	ws.close();
}

int main()
{
	try
	{
		ws();
	}
	catch (Exception::exception &exec)
	{
		std::cout << exec.message << std::endl;
		for (DWORD i = 0; i < exec.count; i++)
		{
			Exception::exception::frame &frame = exec.stack[i];
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
						<< ']'
						<< std::endl;
		}
	}
}
