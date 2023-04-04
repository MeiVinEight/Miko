#ifndef STREAMING_H
#define STREAMING_H

#ifdef STREAMING_SHARED
	#define STREAMINGAPI __declspec(dllexport)
#else
	#define STREAMINGAPI __declspec(dllimport)
#endif

#include <memory.h>
#include <sstring.h>

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
	class format: public Streaming::stream
	{
		private:
		DWORD temporary = 0xFFFFFFFF;
		public:
		Streaming::stream *stream;
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
		STREAMINGAPI Streaming::format &operator<<(char);
		STREAMINGAPI Streaming::format &operator<<(int);
		STREAMINGAPI Streaming::format &operator<<(DWORD);
		STREAMINGAPI Streaming::format &operator<<(QWORD);
		STREAMINGAPI Streaming::format &operator<<(void *);
		STREAMINGAPI Streaming::format &operator<<(const char *);
		STREAMINGAPI Streaming::format &operator<<(const String::string &);
		STREAMINGAPI Streaming::format &operator<<(void (*)(Streaming::stream *));
	};
	STREAMINGAPI void LF(Streaming::stream *);
}

#endif //STREAMING_H
