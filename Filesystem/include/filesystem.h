#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#ifdef FS_SHARED
	#define FSAPI __declspec(dllexport)
#else
	#define FSAPI __declspec(dllimport)
#endif

#include <memory.h>
#include <sstring.h>
#include <streaming.h>

/*
 * TODO UNC ?
 */
namespace Filesystem
{
	const static QWORD FILE_ERROR = -1;
	const static DWORD OF_SHARE_COMPAT     = 0x00000000;
	const static DWORD OF_READ             = 0x00000000;
	const static DWORD OF_WRITE            = 0x00000001;
	const static DWORD OF_READWRITE        = 0x00000002;
	const static DWORD OF_SHARE_EXCLUSIVE  = 0x00000010;
	const static DWORD OF_SHARE_DENY_WRITE = 0x00000020;
	const static DWORD OF_SHARE_DENY_READ  = 0x00000030;
	const static DWORD OF_SHARE_DENY_NONE  = 0x00000040;
	const static DWORD OF_PARSE            = 0x00000100;
	const static DWORD OF_DELETE           = 0x00000200;
	const static DWORD OF_VERIFY           = 0x00000400;
	const static DWORD OF_CANCEL           = 0x00000800;
	const static DWORD OF_CREATE           = 0x00001000;
	const static DWORD OF_PROMPT           = 0x00002000;
	const static DWORD OF_EXIST            = 0x00004000;
	const static DWORD OF_REOPEN           = 0x00008000;
	FSAPI extern const QWORD STDIN;
	FSAPI extern const QWORD STDOUT;

	FSAPI bool create(const String::string &);
	FSAPI bool make(const String::string &);
	FSAPI bool remove(const String::string &);
	FSAPI bool exist(const String::string &);
	FSAPI bool file(const String::string &);
	FSAPI bool directory(const String::string &);
	FSAPI Memory::string parent(const String::string &);
	FSAPI Memory::string canonicalize(const String::string &);
	FSAPI QWORD open(const String::string &, DWORD);
	FSAPI void close(QWORD);
	FSAPI DWORD read(QWORD, void *, DWORD);
	FSAPI DWORD write(QWORD, const void *, DWORD);
	FSAPI void seek(QWORD, QWORD, DWORD);
	FSAPI void flush(QWORD);
}

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
		FSAPI void read(void *, DWORD) override;
		FSAPI void write(const void *, DWORD) override;
		FSAPI void flush() override;
		FSAPI QWORD available() override;
		FSAPI Streaming::file &operator=(Streaming::file &&) noexcept;
		FSAPI virtual Streaming::file &operator>>(const Memory::string &);
		FSAPI virtual Streaming::file &operator<<(const Memory::string &);
		FSAPI virtual void seek(QWORD) const;
		FSAPI virtual void close();
	};
	FSAPI extern Streaming::file input;
	FSAPI extern Streaming::file output;
	FSAPI extern Streaming::format cin;
	FSAPI extern Streaming::format cout;
}

#endif //FILESYSTEM_H
