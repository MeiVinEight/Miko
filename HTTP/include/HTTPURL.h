#ifndef HTTPURL_H
#define HTTPURL_H

#include <HTTPSec.h>
#include <WinType.h>
#include <StringURL.h>

namespace HTTP
{

	class URL: public String::URL
	{
		public:
		String::string host;
		WORD port = 443;
		bool special = false;
		String::string path;

		HTTPAPI void resolve(const String::string &) override;
	};
}

#endif //HTTPURL_H
