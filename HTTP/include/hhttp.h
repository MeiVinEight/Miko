#ifndef HTTP_H
#define HTTP_H

#ifdef HTTP_SHARED
	#define HTTPAPI __declspec(dllexport)
#else
	#define HTTPAPI __declspec(dllimport)
#endif

#include <memory.h>
#include <sstring.h>
#include <wsa.h>

namespace HTTP
{
	HTTPAPI extern const WORD RM_GET;
	HTTPAPI extern const WORD RM_HEAD;
	HTTPAPI extern const WORD RM_POST;
	HTTPAPI extern const WORD RM_PUT;
	HTTPAPI extern const WORD RM_DELETE;
	HTTPAPI extern const WORD RM_CONNECT;
	HTTPAPI extern const WORD RM_OPTIONS;
	HTTPAPI extern const WORD RM_TRACE;
	HTTPAPI extern const WORD RM_PATCH;

	HTTPAPI extern const WORD HV_0_9;
	HTTPAPI extern const WORD HV_1_0;
	HTTPAPI extern const WORD HV_1_1;
	HTTPAPI extern const WORD HV_2_0;

	class URL: public String::URL
	{
		public:
		String::string host;
		WORD port = 80;
		String::string path;

		HTTPAPI URL(const String::string &);
	};
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

		HTTPAPI Message();
		HTTPAPI Message(const HTTP::Message &);
		HTTPAPI Message(HTTP::Message &&) noexcept;
		HTTPAPI ~Message();
		HTTPAPI HTTP::Message &operator=(const HTTP::Message &);
		HTTPAPI HTTP::Message &operator=(HTTP::Message &&) noexcept;
		HTTPAPI String::string &operator[](const String::string &);
		HTTPAPI String::string *operator[](QWORD) const;
		HTTPAPI bool contain(const String::string &) const;
	};

	class ConnectionManager
	{
		public:
		WSA::Socket connection;

		ConnectionManager() = delete;
		ConnectionManager(const HTTP::ConnectionManager &) = delete;
		HTTP::ConnectionManager &operator=(const HTTP::ConnectionManager &) = delete;

		HTTPAPI ConnectionManager(const WSA::SocketAddress &);
		HTTPAPI ConnectionManager(HTTP::ConnectionManager &&) noexcept;
		HTTPAPI ~ConnectionManager();
		HTTPAPI HTTP::ConnectionManager &operator=(HTTP::ConnectionManager &&) noexcept;
		HTTPAPI void send(const HTTP::Message &);
		HTTPAPI HTTP::Message accept();
		HTTPAPI void close();
	};

	HTTPAPI String::string method(WORD);
	HTTPAPI String::string status(WORD);
}

#endif //HTTP_H
