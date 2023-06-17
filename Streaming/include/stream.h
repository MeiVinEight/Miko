#ifndef STREAM_H
#define STREAM_H

#include <streamingapi.h>
#include <memory.h>

namespace Streaming
{
	class stream
	{
		public:
		STREAMINGAPI virtual DWORD read(void *, DWORD) = 0;
		STREAMINGAPI virtual DWORD read(const Memory::string &);
		STREAMINGAPI virtual DWORD read(const Memory::string &, DWORD);
		STREAMINGAPI virtual DWORD write(const void *, DWORD) = 0;
		STREAMINGAPI virtual DWORD write(const Memory::string &);
		STREAMINGAPI virtual DWORD write(const Memory::string &, DWORD);
		STREAMINGAPI virtual void flush() = 0;
		STREAMINGAPI virtual QWORD available() = 0;
		// TODO undo support
	};
}

#endif //STREAM_H
