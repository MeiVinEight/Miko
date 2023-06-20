#ifndef CWEBSOCKET_H
#define CWEBSOCKET_H

#include <CWSSpec.h>
#include <WSMessage.h>
#include <Socket.h>
#include <RNG.h>

namespace CWS
{
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
		CWSAPI void send(const CWS::Message &);
	};
}

#endif //CWEBSOCKET_H
