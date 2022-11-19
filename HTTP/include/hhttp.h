#ifndef HTTP_H
#define HTTP_H

#ifdef HTTP_SHARED
	#define HTTP_API __declspec(dllexport)
#else
	#define HTTP_API
#endif

#include <wsa.h>

namespace HTTP
{
	const WORD RM_GET		= 1;
	const WORD RM_HEAD		= 2;
	const WORD RM_POST		= 3;
	const WORD RM_PUT		= 4;
	const WORD RM_DELETE	= 5;
	const WORD RM_CONNECT	= 6;
	const WORD RM_OPTIONS	= 7;
	const WORD RM_TRACE		= 8;
	const WORD RM_PATCH		= 9;

	const WORD HV_0_9		= ((0 << 8) | 9);
	const WORD HV_1_0		= ((1 << 8) | 0);
	const WORD HV_1_1		= ((1 << 8) | 1);
	const WORD HV_2_0		= ((2 << 8) | 0);

	String::string method(WORD);

	class Message
	{
		public:
		WORD method = 0;
		String::string URL;
		WORD version = HTTP::HV_1_1;
		WORD status = 0;
		String::string (*context)[2] = nullptr;
		QWORD length = 0;
		Memory::string content = Memory::string(0);

		HTTP_API
		Message();
		HTTP_API
		Message(const HTTP::Message &);
		HTTP_API
		Message(HTTP::Message &&);
		HTTP_API
		~Message();
		HTTP_API
		HTTP::Message &operator=(const HTTP::Message &);
		HTTP_API
		HTTP::Message &operator=(HTTP::Message &&);
		HTTP_API
		String::string &operator[](const String::string &);
		HTTP_API
		String::string *operator[](QWORD) const;
		HTTP_API
		bool contain(const String::string &) const;
	};

	class ConnectionManager
	{
		public:
		WSA::Socket connection;

		ConnectionManager() = delete;
		ConnectionManager(const HTTP::ConnectionManager &) = delete;
		HTTP::ConnectionManager &operator=(const HTTP::ConnectionManager &) = delete;

		HTTP_API
		ConnectionManager(const WSA::SocketAddress &);
		HTTP_API
		ConnectionManager(HTTP::ConnectionManager &&);
		HTTP_API
		HTTP::ConnectionManager &operator=(HTTP::ConnectionManager &&);
		HTTP_API
		void send(const HTTP::Message &);
		HTTP_API
		HTTP::Message accept();
	};
}

#endif //HTTP_H
