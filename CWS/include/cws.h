#ifndef CWS_H
#define CWS_H

#include <CWSSpec.h>
#include <WinType.h>
#include <memory.h>

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
}

#endif //CWS_H
