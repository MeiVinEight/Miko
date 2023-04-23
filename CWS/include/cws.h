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
// TODO WS + TLS -> WSS
// TODO WS URL
namespace CWS
{
	static const BYTE OPC_CONTINUATION = 0x0;
	static const BYTE OPC_TEXT         = 0x1;
	static const BYTE OPC_BINARY       = 0x2;
	static const BYTE OPC_CLOSE        = 0x8;
	static const BYTE OPC_PING         = 0x9;
	static const BYTE OPC_PONG         = 0xA;

	static const DWORD CLOSE_NORMAL_CLOSURE      = 1000;
	static const DWORD CLOSE_GOING_AWAY          = 1001;
	static const DWORD CLOSE_PROTOCOL_ERROR      = 1002;
	static const DWORD CLOSE_REJECT              = 1003;
	static const DWORD CLOSE_TYPE_NOT_CONSISTENT = 1007;
	static const DWORD CLOSE_VIOLATION_MESSAGE   = 1008;
	static const DWORD CLOSE_MESSAGE_TOO_BIG     = 1009;
	static const DWORD CLOSE_EXPECTED_EXTENSION  = 1010;
	static const DWORD CLOSE_PREVENTED           = 1011;

	CWSAPI Memory::string security(const Memory::string &);
	CWSAPI bool verify(const Memory::string &, const Memory::string &);

	class URL: public String::URL
	{
		public:
		String::string address;
		WORD ID = 443;
		bool special = false;
		String::string path;

		CWSAPI void resolve(const String::string &) override;
	};
	class Message
	{
		public:
		BYTE FIN = 1;
		BYTE RSV = 0;
		BYTE MSK = 1;
		BYTE OPC = CWS::OPC_BINARY;
		Memory::string context;
	};
	class WebSocket
	{
		public:
		WSA::Socket *connection = nullptr;
		Cryptography::RNG *random = nullptr;
		bool opening = true;
		CWS::Message control;

		CWSAPI WebSocket();
		CWSAPI WebSocket(const CWS::WebSocket &);
		CWSAPI WebSocket(CWS::WebSocket &&) noexcept;
		CWSAPI virtual ~WebSocket();
		CWSAPI CWS::WebSocket &operator=(const CWS::WebSocket &);
		CWSAPI CWS::WebSocket &operator=(CWS::WebSocket &&) noexcept;
		CWSAPI bool alive() const;
		CWSAPI CWS::Message frame() const;
		CWSAPI void frame(const CWS::Message &) const;
		CWSAPI CWS::Message receive();
		CWSAPI void transmit(const CWS::Message &);
	};
}

#endif //CWS_H
