#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include <HTTPSec.h>
#include <WinType.h>
#include <sstring.h>
#include <hhttp.h>

namespace HTTP
{
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
}

#endif //HTTPMESSAGE_H
