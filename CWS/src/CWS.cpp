#include "definitions.h"

const BYTE CWS::OPC_CONTINUATION = 0x0;
const BYTE CWS::OPC_TEXT         = 0x1;
const BYTE CWS::OPC_BINARY       = 0x2;
const BYTE CWS::OPC_CLOSE        = 0x8;
const BYTE CWS::OPC_PING         = 0x9;
const BYTE CWS::OPC_PONG         = 0xA;

bool CWS::verify(const Memory::string &key, const Memory::string &accept)
{
	Memory::string expected = SecurityKey(key);
	if (expected.length == accept.length)
	{
		return Memory::compare(expected.address, accept.address, expected.length);
	}
	return false;
}