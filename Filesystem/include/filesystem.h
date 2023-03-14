#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#ifdef FS_SHARED
	#define FSAPI __declspec(dllexport)
#else
	#define FSAPI
#endif

#include <memory.h>
#include <sstring.h>


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
namespace Filesystem
{
	FSAPI
	bool create(const String::string &);
	FSAPI
	bool make(const String::string &);
	FSAPI
	bool remove(const String::string &);
	FSAPI
	bool exist(const String::string &);
	FSAPI
	bool file(const String::string &);
	FSAPI
	bool directory(const String::string &);
	FSAPI
	Memory::string parent(const String::string &);
	FSAPI
	Memory::string canonicalize(const String::string &);
	FSAPI
	QWORD open(const String::string &, DWORD);
	FSAPI
	void close(QWORD);
	FSAPI
	DWORD read(QWORD, void *, DWORD);
	FSAPI
	DWORD write(QWORD, void *, DWORD);
	FSAPI
	void seek(QWORD, QWORD, DWORD);

	class AbstractStream
	{
		public:
		FSAPI
		virtual void read(void *, DWORD) = 0;
		FSAPI
		virtual void write(const void *, DWORD) = 0;
		FSAPI
		virtual QWORD available() = 0;
		/**
		 * @TODO unread support
		 */
	};

	class FileStream: public Filesystem::AbstractStream
	{
		public:
		QWORD file = HFILE_ERROR;

		FSAPI
		explicit FileStream(const void *);
		FSAPI
		explicit FileStream(QWORD);
		FSAPI
		FileStream(FileStream &&) noexcept;
		FSAPI
		~FileStream();
		FSAPI
		Filesystem::FileStream &operator=(Filesystem::FileStream &&) noexcept;
		FSAPI
		void read(void *, DWORD) override;
		FSAPI
		void write(const void *, DWORD) override;
		FSAPI
		QWORD available() override;
		FSAPI
		void seek(QWORD) const;
		FSAPI
		void close();
	};
}

#endif //FILESYSTEM_H
