#define MEMORY_SHARED

#include "memory.h"

typedef unsigned __int64 size_t;

#ifdef __cplusplus
extern "C"
{
#endif
void* __cdecl malloc(size_t);
void* __cdecl realloc(void* , size_t);
void __cdecl free(void*);
void* __cdecl memcpy(void *, void const*, size_t);
void* __cdecl memset(void *, int, size_t);

#ifdef __cplusplus
}
#endif

void *Memory::allocate(QWORD size)
{
	return malloc(size);
}

void *Memory::reallocate(void *p, QWORD size)
{
	return realloc(p, size);
}

void Memory::free(void *p)
{
	::free(p);
}

void Memory::copy(void *dst, const void *src, QWORD len)
{
	memcpy(dst, src, len);
}

void Memory::fill(void * p, BYTE x, QWORD size)
{
	memset(p, x, size);
}

Memory::string::string(): Memory::string::string(1)
{
}

Memory::string::string(QWORD size): address(Memory::allocate(size)), memory(size)
{
}

Memory::string::~string()
{
	if (this->address)
	{
		Memory::free(this->address);
		this->address = NULL;
	}
	this->memory = 0;
}

BYTE &Memory::string::operator[](QWORD off) const
{
	return ((BYTE *)this->address)[off];
}

void Memory::string::ensure(QWORD size)
{
	if (this->memory < size)
	{
		this->address = Memory::reallocate(this->address, size);
		this->memory = size;
	}
}
