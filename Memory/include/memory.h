#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#ifdef MEMORY_SHARED
	#define MEMORYAPI __declspec(dllexport)
#else
	#define MEMORYAPI
#endif

#ifndef NULL
#define NULL 0
#endif

typedef unsigned long long QWORD;
typedef unsigned char BYTE;


namespace Memory
{
	MEMORYAPI
	void *allocate(QWORD);
	MEMORYAPI
	void *reallocate(void *, QWORD);
	MEMORYAPI
	void free(void *);
	MEMORYAPI
	void copy(void *, const void *, QWORD);
	MEMORYAPI
	void fill(void *, BYTE, QWORD);
	MEMORYAPI
	bool compare(const void *, const void *, QWORD);
	MEMORYAPI
	void violation();

	class string
	{
		public:
		QWORD length = 0;
		char *address = nullptr;

		string() = delete;
		MEMORYAPI
		explicit string(QWORD);
		MEMORYAPI
		string(const Memory::string &);
		MEMORYAPI
		string(Memory::string &&) noexcept;
		MEMORYAPI
		~string();
		MEMORYAPI
		Memory::string &operator=(const Memory::string &);
		MEMORYAPI
		Memory::string &operator=(Memory::string &&) noexcept;
		MEMORYAPI
		BYTE &operator[](QWORD) const;
		MEMORYAPI
		void resize(QWORD);
	};
}

#endif //MEMORY_MEMORY_H
