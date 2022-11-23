#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#ifdef MEMORY_SHARED
	#define MEMORY_API __declspec(dllexport)
#else
	#define MEMORY_API
#endif

#ifndef NULL
#define NULL 0
#endif

typedef unsigned long long QWORD;
typedef unsigned char BYTE;


namespace Memory
{
	MEMORY_API
	void *allocate(QWORD);
	MEMORY_API
	void *reallocate(void *, QWORD);
	MEMORY_API
	void free(void *);
	MEMORY_API
	void copy(void *, const void *, QWORD);
	MEMORY_API
	void fill(void *, BYTE, QWORD);
	MEMORY_API
	bool compare(const void *, const void *, QWORD);

	class string
	{
		public:
		BYTE *address = NULL;
		QWORD length = 0;

		string() = delete;
		MEMORY_API
		explicit string(QWORD);
		MEMORY_API
		string(const Memory::string &);
		MEMORY_API
		string(Memory::string &&);
		MEMORY_API
		~string();
		MEMORY_API
		Memory::string &operator=(const Memory::string &);
		MEMORY_API
		Memory::string &operator=(Memory::string &&);
		MEMORY_API
		BYTE &operator[](QWORD) const;
		MEMORY_API
		operator char *() const;
		MEMORY_API
		void resize(QWORD);
	};
}

#endif //MEMORY_MEMORY_H
