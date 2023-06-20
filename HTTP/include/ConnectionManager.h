#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <WinType.h>
#include <Socket.h>
#include <HTTPSec.h>
#include <HTTPURL.h>
#include <hhttp.h>
#include <HTTPMessage.h>

namespace HTTP
{
	class ConnectionManager
	{
		public:
		HTTP::URL URL;
		WSA::Socket *connection = nullptr;
		BYTE bound = HTTP::HTTP_CLIENT;

		HTTPAPI void transmit(const HTTP::Message &) const;
		HTTPAPI HTTP::Message receive() const;
	};
}

#endif //CONNECTIONMANAGER_H
