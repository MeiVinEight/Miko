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
	class file: Streaming::stream
	{
		public:
		QWORD object = Filesystem::FILE_ERROR;

		STREAMINGAPI file(QWORD);
		STREAMINGAPI file(const String::string &);
		STREAMINGAPI file(Streaming::file &&) noexcept;
		STREAMINGAPI ~file();
		STREAMINGAPI Streaming::file &operator=(Streaming::file &&) noexcept;
		STREAMINGAPI void read(void *, DWORD) override;
		STREAMINGAPI void write(const void *, DWORD) override;
		STREAMINGAPI void flush() override;
		STREAMINGAPI QWORD available() override;
		STREAMINGAPI void seek(QWORD) const;
		STREAMINGAPI void close();
	};
}

#endif //STREAMING_H
