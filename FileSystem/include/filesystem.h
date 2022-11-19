#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#ifdef FS_SHARED
	#define FS_API __declspec(dllexport)
#else
	#define FS_API
#endif

#include <memory.h>


#ifndef HFILE_ERROR
	#define HFILE_ERROR ((HFILE)-1)
#endif

typedef int HFILE;
typedef unsigned long DWORD;
typedef unsigned long long QWORD;

/*
 * TODO Reparse points ?
 * TODO UNC ?
 */
namespace FileSystem
{
	typedef QWORD FD;
	static const QWORD FILE_EOF = -1;
	static const DWORD FILE_CLOSED = 3;

	FS_API
	bool create(const void *);
	FS_API
	bool make(const void *);
	FS_API
	bool remove(const void *);
	FS_API
	bool exist(const void *);
	FS_API
	bool file(const void *);
	FS_API
	bool directory(const void *);
	FS_API
	Memory::string parent(const void *);
	FS_API
	Memory::string canonicalize(const void *);
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

	class AbstractStream
	{
		public:
		FS_API
		virtual void read(void *, DWORD) = 0;
		FS_API
		virtual void write(const void *, DWORD) = 0;
		FS_API
		virtual QWORD available() = 0;
		/**
		 * @TODO unread support
		 */
	};

	class FileStream: public FileSystem::AbstractStream
	{
		public:
		FileSystem::FD file = HFILE_ERROR;

		FS_API
		explicit FileStream(const void *);
		FS_API
		explicit FileStream(FileSystem::FD);
		FS_API
		FileStream(FileStream &&) noexcept;
		FS_API
		~FileStream();
		FS_API
		FileSystem::FileStream &operator=(FileSystem::FileStream &&) noexcept;
		FS_API
		void read(void *, DWORD) override;
		FS_API
		void write(const void *, DWORD) override;
		FS_API
		QWORD available() override;
		FS_API
		void seek(QWORD) const;
		FS_API
		void close();
	};
}

#endif //FILESYSTEM_H
