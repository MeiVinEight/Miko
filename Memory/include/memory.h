#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#ifdef MEMORY_SHARED
	#define MEMORYAPI __declspec(dllexport)
#else
	#define MEMORYAPI __declspec(dllimport)
#endif

typedef int					BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;


namespace Memory
{
	static const BYTE NTSTATUS = 0;
	static const BYTE DOSERROR = 1;
	static const BYTE EXTERNAL = 2;

	MEMORYAPI void *allocate(QWORD);
	MEMORYAPI void *reallocate(void *, QWORD);
	MEMORYAPI void free(void *);
	MEMORYAPI void copy(void *, const void *, QWORD);
	MEMORYAPI void fill(void *, BYTE, QWORD);
	MEMORYAPI bool compare(const void *, const void *, QWORD);
	MEMORYAPI void reverse(void *, QWORD);
	MEMORYAPI void violation();

	class string
	{
		public:
		QWORD length = 0;
		BYTE *address = nullptr;

		MEMORYAPI string();
		MEMORYAPI explicit string(QWORD);
		MEMORYAPI string(const Memory::string &);
		MEMORYAPI string(Memory::string &&) noexcept;
		MEMORYAPI ~string();
		MEMORYAPI Memory::string &operator=(const Memory::string &);
		MEMORYAPI Memory::string &operator=(Memory::string &&) noexcept;
		MEMORYAPI BYTE &operator[](QWORD) const;
		MEMORYAPI void resize(QWORD);
	};
	class exception
	{
		public:
		class frame
		{
			public:
			void *address = nullptr;
			void *offset = nullptr;
			void *module = nullptr;
			Memory::string function;
			Memory::string library;

			frame() = delete;
			MEMORYAPI frame(void *);
			MEMORYAPI frame(const Memory::exception::frame &);
			MEMORYAPI ~frame();
		};

		DWORD code = 0;
		BYTE type = Memory::DOSERROR;
		Memory::string message;
		Memory::exception::frame *stack = nullptr;
		DWORD count = 0;
		MEMORYAPI exception(DWORD, BYTE);
		MEMORYAPI exception(DWORD);
		MEMORYAPI exception(const Memory::exception &) noexcept;
		MEMORYAPI ~exception();
	};
	namespace BE
	{
		MEMORYAPI QWORD get(const void *, BYTE);
		MEMORYAPI WORD get(WORD);
		MEMORYAPI DWORD get(DWORD);
		MEMORYAPI QWORD get(QWORD);
		MEMORYAPI void set(QWORD, void *, BYTE);
	}
	namespace LE
	{
		MEMORYAPI QWORD get(const void *, BYTE);
		MEMORYAPI QWORD get(WORD);
		MEMORYAPI QWORD get(DWORD);
		MEMORYAPI QWORD get(QWORD);
		MEMORYAPI void set(QWORD, void *, BYTE);
	}

	MEMORYAPI extern const BYTE BENDIAN;
	MEMORYAPI extern const BYTE LENDIAN;
	MEMORYAPI extern const DWORD ERRNO_SUCCESS;
	MEMORYAPI extern const DWORD ERRNO_ACCESS_VIOLATION;
	MEMORYAPI extern const DWORD ERRNO_INVALID_PARAMETER;
	MEMORYAPI extern const DWORD ERRNO_OBJECT_CLOSED;

	MEMORYAPI DWORD registry(const char *);
	MEMORYAPI Memory::string message(DWORD, BYTE);
}

#endif //MEMORY_MEMORY_H
