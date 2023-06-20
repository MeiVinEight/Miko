#ifndef STRINGURL_H
#define STRINGURL_H

#include <stringspec.h>
#include <WinType.h>
#include <sstring.h>

namespace String
{
	class URL
	{
		public:
		String::string location;
		String::string scheme;
		String::string specific;

		STRINGAPI virtual ~URL() noexcept;
		STRINGAPI virtual void resolve(const String::string &);
	};
}

#endif //STRINGURL_H
