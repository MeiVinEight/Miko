#ifndef CWS_H
#define CWS_H

#ifdef CWS_SHARED
	#define CWSAPI __declspec(dllexport)
#else
	#define CWSAPI
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
// TODO TLS->WSS: OpenSSL
namespace CWS
{
	class WebSocket
	{
		public:
		HTTP::ConnectionManager manager;
		HTTP::Message message;
		String::string key;
		String::string verification;

		WebSocket() = delete;
		WebSocket(const WebSocket &) = delete;
		CWS::WebSocket &operator=(const WebSocket &) = delete;

		CWSAPI
		WebSocket(const WSA::SocketAddress &, const String::string &);
		CWSAPI
		WebSocket(CWS::WebSocket &&) noexcept;
		CWSAPI
		~WebSocket();
		CWSAPI
		bool alive();
		CWSAPI
		WSA::Socket &connection();
		CWSAPI
		CWS::WebSocket &operator=(WebSocket &&) noexcept;
		CWSAPI
		Memory::string accept();
		CWSAPI
		void send(const Memory::string &);
		CWSAPI
		void close();
	};
}

#endif //CWS_H
