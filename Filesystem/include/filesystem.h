#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <memory.h>
#include <sstring.h>
#include <fsspec.h>

/*
 * TODO UNC ?
 */
namespace Filesystem
{
	const static QWORD FILE_ERROR = -1;

	const static DWORD FA_READ_DATA            = 0x00000001;
	const static DWORD FA_LIST_DIRECTORY       = 0x00000001;
	const static DWORD FA_WRITE_DATA           = 0x00000002;
	const static DWORD FA_ADD_FILE             = 0x00000002;
	const static DWORD FA_ADD_SUBDIRECTORY     = 0x00000004;
	const static DWORD FA_APPEND_DATA          = 0x00000004;
	const static DWORD FA_CREATE_PIPE_INSTANCE = 0x00000004;
	const static DWORD FA_READ_EA              = 0x00000008;
	const static DWORD FA_WRITE_EA             = 0x00000010;
	const static DWORD FA_EXECUTE              = 0x00000020;
	const static DWORD FA_TRAVERSE             = 0x00000020;
	const static DWORD FA_DELETE_CHILD         = 0x00000040;
	const static DWORD FA_READ_ATTRIBUTES      = 0x00000080;
	const static DWORD FA_WRITE_ATTRIBUTES     = 0x00000100;

	const static DWORD FILE_SHARE_READ         = 0x00000001;
	const static DWORD FILE_SHARE_WRITE        = 0x00000002;
	const static DWORD FILE_SHARE_DELETE       = 0x00000004;

	const static DWORD CREATE_NEW              = 1;
	const static DWORD CREATE_ALWAYS           = 2;
	const static DWORD OPEN_EXISTING           = 3;
	const static DWORD OPEN_ALWAYS             = 4;
	const static DWORD TRUNCATE_EXISTING       = 5;

	const static DWORD SEEK_BEGIN              = 0;
	const static DWORD SEEK_CURRENT            = 1;
	const static DWORD SEEk_END                = 2;

	FSAPI extern const DWORD ERRNO_WRONG_FILE_TYPE;
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
	FSAPI bool absolute(const String::string &);
	FSAPI QWORD open(const String::string &, DWORD);
	FSAPI QWORD open(const String::string &, DWORD, DWORD);
	FSAPI void close(QWORD);
	FSAPI DWORD read(QWORD, void *, DWORD);
	FSAPI DWORD write(QWORD, const void *, DWORD);
	FSAPI void seek(QWORD, QWORD, DWORD);
	FSAPI void flush(QWORD);
	FSAPI QWORD available(QWORD);
}


#endif //FILESYSTEM_H
