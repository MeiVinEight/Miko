#ifndef FULLY_H
#define FULLY_H

#include <streamingapi.h>
#include <stream.h>

namespace Streaming
{
	class fully: public Streaming::stream
	{
		public:
		Streaming::stream *stream;
		STREAMINGAPI fully(Streaming::stream *);
		STREAMINGAPI DWORD read(void *, DWORD) override;
		STREAMINGAPI DWORD read(const Memory::string &) override;
		STREAMINGAPI DWORD read(const Memory::string &, DWORD) override;
		STREAMINGAPI DWORD write(const void *, DWORD) override;
		STREAMINGAPI DWORD write(const Memory::string &) override;
		STREAMINGAPI DWORD write(const Memory::string &, DWORD) override;
		STREAMINGAPI void flush() override;
		STREAMINGAPI QWORD available() override;
	};
}

#endif //FULLY_H
