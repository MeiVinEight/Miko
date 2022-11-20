#ifndef CWS_H
#define CWS_H

#ifdef CWS_SHARED
	#define CWS_API __declspec(dllexport)
#else
	#define CWS_API
#endif

#include <memory.h>
#include <wsa.h>
#include <hhttp.h>

/*
+---------------------------------------------------------------+
|0               1               2               3              |
|0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7|
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+
*/
// TLS->WSS: OpenSSL
namespace CWS
{
	class WebSocket
	{
		public:
		HTTP::ConnectionManager manager;
		WSA::Socket &connection = manager.connection;

		WebSocket() = delete;
		WebSocket(const WebSocket &) = delete;
		CWS::WebSocket &operator=(const WebSocket &) = delete;

		CWS_API
		WebSocket(const WSA::SocketAddress &, const String::string &);
		CWS_API
		WebSocket(CWS::WebSocket &&);
		CWS_API
		~WebSocket();
		CWS_API
		CWS::WebSocket &operator=(WebSocket &&);
		CWS_API
		Memory::string accept();
		CWS_API
		void send(const Memory::string &);
		CWS_API
		void close();
	};
}

#endif //CWS_H
