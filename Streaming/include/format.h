#ifndef FORMAT_H
#define FORMAT_H

#include <streamingapi.h>
#include <WinType.h>
#include <stream.h>
#include <memory.h>
#include <sstring.h>
#include <exception.h>

namespace Streaming
{
	class format: public Streaming::stream
	{
		private:
		DWORD temporary = 0xFFFFFFFF;
		Streaming::stream *stream;
		public:
		STREAMINGAPI format(Streaming::stream *);
		STREAMINGAPI DWORD read(void *, DWORD) override;
		STREAMINGAPI DWORD write(const void *, DWORD) override;
		STREAMINGAPI void flush() override;
		STREAMINGAPI QWORD available() override;
		STREAMINGAPI virtual Streaming::format &operator>>(const Memory::string &);
		STREAMINGAPI virtual Streaming::format &operator<<(const Memory::string &);
		STREAMINGAPI Streaming::format &operator>>(char &);
		STREAMINGAPI Streaming::format &operator>>(int &);
		STREAMINGAPI Streaming::format &operator>>(DWORD &);
		STREAMINGAPI Streaming::format &operator>>(QWORD &);
		STREAMINGAPI Streaming::format &operator>>(String::string &);
		STREAMINGAPI Streaming::format &operator<<(bool);
		STREAMINGAPI Streaming::format &operator<<(char);
		STREAMINGAPI Streaming::format &operator<<(int);
		STREAMINGAPI Streaming::format &operator<<(DWORD);
		STREAMINGAPI Streaming::format &operator<<(QWORD);
		STREAMINGAPI Streaming::format &operator<<(void *);
		STREAMINGAPI Streaming::format &operator<<(const char *);
		STREAMINGAPI Streaming::format &operator<<(const String::string &);
		STREAMINGAPI Streaming::format &operator<<(const Memory::exception &);
		STREAMINGAPI Streaming::format &operator<<(void (*)(Streaming::stream *));
	};
}

#endif //FORMAT_H
