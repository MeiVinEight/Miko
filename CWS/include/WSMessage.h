#ifndef WSMESSAGE_H
#define WSMESSAGE_H

#include <WinType.h>
#include <cws.h>
#include <memory.h>

namespace CWS
{
	class Message
	{
		public:
		BYTE FIN = 1;
		BYTE RSV = 0;
		BYTE MSK = 1;
		BYTE OPC = CWS::OPC_BINARY;
		Memory::string context;
	};
}

#endif //WSMESSAGE_H
