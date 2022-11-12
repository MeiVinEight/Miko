#ifndef WSA_FS_H
#define WSA_FS_H

#include "wsadef.h"

#ifndef HFILE_ERROR
#define HFILE_ERROR ((HFILE)-1)
#endif

typedef int HFILE;
typedef const char *LPCSTR;

namespace WSA
{
	typedef QWORD FD;
	static const QWORD FILE_EOF = -1;
	static const DWORD FILE_CLOSED = 3;

	WSA_API
	BOOL create(LPCSTR);
	WSA_API
	BYTE exist(LPCSTR);
	WSA_API
	WSA::FD open(LPCSTR, DWORD);
	WSA_API
	void close(WSA::FD);
	WSA_API
	DWORD read(WSA::FD, BYTE *, DWORD);
	WSA_API
	DWORD write(WSA::FD, BYTE *, DWORD);
	WSA_API
	void seek(WSA::FD, QWORD, DWORD);

	class BIO
	{
		public:
		WSA_API
		virtual DWORD read(BYTE *, DWORD) = 0;
		WSA_API
		virtual DWORD write(BYTE *, DWORD) = 0;
		WSA_API
		virtual QWORD available() = 0;
		/**
		 * @TODO unread support
		 */
	};

	class FIO: public BIO
	{
		public:
		WSA::FD file = HFILE_ERROR;

		WSA_API
		explicit FIO(LPCSTR);
		WSA_API
		explicit FIO(WSA::FD);
		WSA_API
		FIO(FIO &&) noexcept;
		WSA_API
		~FIO();
		WSA_API
		WSA::FIO &operator=(WSA::FIO &&) noexcept;
		WSA_API
		DWORD read(BYTE *, DWORD) override;
		WSA_API
		DWORD write(BYTE *, DWORD) override;
		WSA_API
		QWORD available() override;
		WSA_API
		void seek(QWORD) const;
		WSA_API
		void close();
	};
}

#endif //WSA_FS_H
