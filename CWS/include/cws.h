#ifndef CWS_H
#define CWS_H

#ifdef CWS_SHARED
	#define CWSAPI __declspec(dllexport)
#else
	#define CWSAPI __declspec(dllimport)
#endif

#include <memory.h>
#include <wsa.h>
#include <crypto.h>

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
// TODO WS URL
namespace CWS
{
	CWSAPI extern const BYTE OPC_CONTINUATION;
	CWSAPI extern const BYTE OPC_TEXT;
	CWSAPI extern const BYTE OPC_BINARY;
	CWSAPI extern const BYTE OPC_CLOSE;
	CWSAPI extern const BYTE OPC_PING;
	CWSAPI extern const BYTE OPC_PONG;

	CWSAPI bool verify(const Memory::string &, const Memory::string &);

	class Message
	{
		public:
		BYTE RSV = 0;
		BYTE OPC = CWS::OPC_BINARY;
		Memory::string context;
	};
	class WebSocket
	{
		public:
		WSA::Socket *connection;
		Cryptography::RNG *random;
		bool opening = true;

		WebSocket() = delete;
		WebSocket(const WebSocket &) = delete;
		CWS::WebSocket &operator=(const WebSocket &) = delete;

		CWSAPI WebSocket(WSA::Socket *);
		CWSAPI WebSocket(CWS::WebSocket &&) noexcept;
		CWSAPI ~WebSocket();
		CWSAPI CWS::WebSocket &operator=(WebSocket &&) noexcept;
		CWSAPI bool alive() const;
		CWSAPI CWS::Message receive() const;
		CWSAPI void send(const CWS::Message &) const;
		CWSAPI void close();
	};
}

#endif //CWS_H
