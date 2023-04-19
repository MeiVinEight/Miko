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

	HTTPAPI extern const DWORD ERRNO_UNKNOWN_REQUEST_METHOD;
	HTTPAPI extern const DWORD ERRNO_UNKNOWN_STATUS_CODE;

	static const BYTE HTTP_CLIENT = 0;
	static const BYTE HTTP_SERVER = 1;

	class URL: public String::URL
	{
		public:
		String::string host;
		WORD port = 80;
		bool special = false;
		String::string path;

		HTTPAPI void resolve(const String::string &) override;
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
		HTTP::URL URL;
		WSA::Socket *connection = nullptr;
		BYTE bound = HTTP::HTTP_CLIENT;

		HTTPAPI void transmit(const HTTP::Message &) const;
		HTTPAPI HTTP::Message receive() const;
	};

	HTTPAPI String::string method(WORD);
	HTTPAPI WORD method(const String::string &);
	HTTPAPI String::string status(WORD);
}

#endif //HTTP_H
