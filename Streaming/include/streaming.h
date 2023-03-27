#ifndef STREAMING_H
#define STREAMING_H

#ifdef STREAMING_SHARED
	#define STREAMINGAPI __declspec(dllexport)
#else
	#define STREAMINGAPI __declspec(dllimport)
#endif

#include <memory.h>
#include <filesystem.h>
#include <exception.h>

namespace Streaming
{
	class stream
	{
		public:
		STREAMINGAPI virtual void read(void *, DWORD) = 0;
		STREAMINGAPI virtual void read(const Memory::string &);
		STREAMINGAPI virtual void read(const Memory::string &, DWORD);
		STREAMINGAPI virtual void write(const void *, DWORD) = 0;
		STREAMINGAPI virtual void write(const Memory::string &);
		STREAMINGAPI virtual void write(const Memory::string &, DWORD);
		STREAMINGAPI virtual void flush() = 0;
		STREAMINGAPI virtual QWORD available() = 0;
		// TODO undo support
	};
	class file: public Streaming::stream
	{
		public:
		QWORD object = Filesystem::FILE_ERROR;

		STREAMINGAPI file(QWORD);
		STREAMINGAPI file(const String::string &);
		STREAMINGAPI file(Streaming::file &&) noexcept;
		STREAMINGAPI virtual ~file();
		STREAMINGAPI Streaming::file &operator=(Streaming::file &&) noexcept;
		STREAMINGAPI void read(void *, DWORD) override;
		STREAMINGAPI virtual Streaming::file &operator>>(const Memory::string &);
		STREAMINGAPI void write(const void *, DWORD) override;
		STREAMINGAPI virtual Streaming::file &operator<<(const Memory::string &);
		STREAMINGAPI void flush() override;
		STREAMINGAPI QWORD available() override;
		STREAMINGAPI virtual void seek(QWORD) const;
		STREAMINGAPI virtual void close();
	};
	class format: public Streaming::file
	{
		private:
		DWORD temporary = 0xFFFFFFFF;
		public:
		STREAMINGAPI format(QWORD);
		STREAMINGAPI void read(void *, DWORD) override;
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
	extern STREAMINGAPI Streaming::format cin;
	extern STREAMINGAPI Streaming::format cout;
	STREAMINGAPI void LF(Streaming::stream *);
}

#endif //STREAMING_H
