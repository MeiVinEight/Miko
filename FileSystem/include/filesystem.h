#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#ifdef FS_SHARED
	#define FS_API __declspec(dllexport)
#else
	#define FS_API
#endif


#ifndef HFILE_ERROR
	#define HFILE_ERROR ((HFILE)-1)
#endif

typedef int HFILE;
typedef unsigned long DWORD;
typedef unsigned long long QWORD;

/*
 * TODO Reparse points ?
 * TODO Directory create/exists/remove
 * TODO Canonical path
 */
namespace FileSystem
{
	typedef QWORD FD;
	static const QWORD FILE_EOF = -1;
	static const DWORD FILE_CLOSED = 3;

	FS_API
	bool create(const void *);
	FS_API
	bool exist(const void *);
	FS_API
	bool remove(const void *);
	FS_API
	FileSystem::FD open(const void *, DWORD);
	FS_API
	void close(FileSystem::FD);
	FS_API
	DWORD read(FileSystem::FD, void *, DWORD);
	FS_API
	DWORD write(FileSystem::FD, void *, DWORD);
	FS_API
	void seek(FileSystem::FD, QWORD, DWORD);

	class AbstractIO
	{
		public:
		FS_API
		virtual DWORD read(void *, DWORD) = 0;
		FS_API
		virtual DWORD write(void *, DWORD) = 0;
		FS_API
		virtual QWORD available() = 0;
		/**
		 * @TODO unread support
		 */
	};

	class File: public FileSystem::AbstractIO
	{
		public:
		FileSystem::FD file = HFILE_ERROR;

		FS_API
		explicit File(const void *);
		FS_API
		explicit File(FileSystem::FD);
		FS_API
		File(File &&) noexcept;
		FS_API
		~File();
		FS_API
		FileSystem::File &operator=(FileSystem::File &&) noexcept;
		FS_API
		DWORD read(void *, DWORD) override;
		FS_API
		DWORD write(void *, DWORD) override;
		FS_API
		QWORD available() override;
		FS_API
		void seek(QWORD) const;
		FS_API
		void close();
	};
}

#endif //FILESYSTEM_H
