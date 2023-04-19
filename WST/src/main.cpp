#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <wsa.h>
#include <cws.h>
#include <hhttp.h>

void func()
{
	bool running = true;
	WSA::ServerSocket server;
	server.bind({127, 0, 0, 1, 12138});
	WSA::Socket socket = server.accept();
	HTTP::ConnectionManager manager;
	manager.connection = &socket;
	manager.bound = HTTP::HTTP_SERVER;
	HTTP::Message message = manager.receive();
	Streaming::cout << HTTP::method(message.method) << ' ' << message.URL << ' ' << "HTTP/";
	Streaming::cout << (message.version >> 8) << '.' << (message.version & 0xFF) << Streaming::LF;
	for (QWORD i = 0; i < message.length; i++)
	{
		Streaming::cout << message[i][0] << ": " << message[i][1] << Streaming::LF;
	}
	Streaming::cout << Streaming::LF << message.content << Streaming::LF;
	Memory::string secKey = message["Sec-WebSocket-Key"].address;
	message = HTTP::Message();
	message.version = HTTP::HV_1_1;
	message.status = 101;
	message["Upgrade"] = "websocket";
	message["Connection"] = "Upgrade";
	message["Sec-WebSocket-Accept"] = CWS::security(secKey);
	manager.transmit(message);
	Streaming::cout << "HTTP/" << (message.version >> 8) << '.' << (message.version & 0xFF) << ' ' << message.status;
	Streaming::cout << ' ' << HTTP::status(message.status) << Streaming::LF;
	for (QWORD i = 0; i < message.length; i++)
	{
		Streaming::cout << message[i][0] << ": " << message[i][1] << Streaming::LF;
	}
	Streaming::cout << Streaming::LF << message.content << Streaming::LF;

	CWS::WebSocket ws;
	ws.connection = &socket;
	CWS::Message msg;
	while (running)
	{
		msg = CWS::Message();
		msg.FIN = 1;
		msg.MSK = 0;
		msg.OPC = CWS::OPC_TEXT;
		msg.context.resize(11);
		Memory::copy(msg.context.address, "Hello World", 11);
		ws.frame(msg);
		msg = ws.frame();
		running &= !!(msg.context[0] - '0');
	}
	msg = CWS::Message();
	msg.MSK = 0;
	msg.OPC = CWS::OPC_CLOSE;
	msg.context.resize(2);
	msg.context[0] = (CWS::CLOSE_NORMAL_CLOSURE >> 8) & 0xFF;
	msg.context[1] = (CWS::CLOSE_NORMAL_CLOSURE >> 0) & 0xFF;
	ws.frame(msg);
	msg = ws.frame();
	DWORD status = msg.context[0] << 8 | msg.context[1];
	Streaming::cout << status << ": ";
	Streaming::cout.write(msg.context.address + 2, msg.context.length - 2);
	Streaming::cout << Streaming::LF;
	socket.close();
}

int main()
{
	try
	{
		func();
	}
	catch (Memory::exception &exec)
	{
		Streaming::cout << exec.message << Streaming::LF;
		for (DWORD i = 0; i < exec.count; i++)
		{
			/*
			Streaming::cout << exec.stack[i].address << ' ';
			Streaming::cout << exec.stack[i].offset << ' ';
			Streaming::cout << exec.stack[i].module << ' ';
			Streaming::cout << exec.stack[i].function << ' ';
			Streaming::cout << exec.stack[i].library << Streaming::LF;
			*/
			Streaming::cout << exec.stack[i].offset << " (";
			if (exec.stack[i].function.length)
			{
				Streaming::cout << exec.stack[i].function << "+";
				Streaming::cout << Hexadecimal::stringify((QWORD) exec.stack[i].offset - (QWORD) exec.stack[i].address);
			}
			else
			{
				Streaming::cout << exec.stack[i].offset;
			}
			Streaming::cout << ") [";
			if (exec.stack[i].module)
			{
				Streaming::cout << exec.stack[i].library << '+';
				Streaming::cout << Hexadecimal::stringify(((QWORD) exec.stack[i].offset) - ((QWORD) exec.stack[i].module));
			}
			else
			{
				Streaming::cout << exec.stack[i].address;
			}
			Streaming::cout << ']' << Streaming::LF;
		}
	}
}