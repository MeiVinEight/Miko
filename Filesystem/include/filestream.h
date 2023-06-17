#ifndef FILESTREAM_H
#define FILESTREAM_H

#include <filesystem.h>
#include <stream.h>
#include <sstring.h>

namespace Streaming
{
	class file: public Streaming::stream
	{
		public:
		QWORD object = Filesystem::FILE_ERROR;

		FSAPI file(QWORD);
		FSAPI file(const String::string &);
		FSAPI file(Streaming::file &&) noexcept;
		FSAPI virtual ~file();
		FSAPI DWORD read(void *, DWORD) override;
		FSAPI DWORD write(const void *, DWORD) override;
		FSAPI void flush() override;
		FSAPI QWORD available() override;
		FSAPI Streaming::file &operator=(Streaming::file &&) noexcept;
		FSAPI virtual Streaming::file &operator>>(const Memory::string &);
		FSAPI virtual Streaming::file &operator<<(const Memory::string &);
		FSAPI virtual void seek(QWORD) const;
		FSAPI virtual void close();
	};
}

#endif //FILESTREAM_H
