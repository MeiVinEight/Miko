#ifndef WSURL_H
#define WSURL_H

#include <CWSSpec.h>
#include <StringURL.h>

namespace CWS
{
	class URL: public String::URL
	{
		public:
		String::string address;
		WORD ID = 443;
		bool special = false;
		String::string path;

		CWSAPI void resolve(const String::string &) override;
	};
}

#endif //WSURL_H
