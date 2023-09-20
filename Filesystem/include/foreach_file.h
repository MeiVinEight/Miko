#include <fsspec.h>
#include <WinType.h>
#include <sstring.h>

namespace Filesystem
{
	class foreach
	{
		public:
		QWORD object = 0;
		String::string name;
		FSAPI ~foreach();
		FSAPI bool operator()(const String::string &);
		FSAPI bool operator()();
		FSAPI bool first(const String::string &);
		FSAPI bool next();
		FSAPI void close();
	};
}