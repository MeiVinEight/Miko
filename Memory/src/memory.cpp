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

bool Memory::compare(const void *p1, const void *p2, QWORD length)
{
	const char *s1 = (const char *)p1;
	const char *s2 = (const char *)p2;
	bool ret = true;
	for (QWORD i = 0; ret && i < length; i++)
	{
		ret &= s1[i] == s2[i];
	}
	return ret;
}

Memory::string::string(QWORD size): address(Memory::allocate(size)), length(size)
{
}

Memory::string::string(const Memory::string &copy): address(Memory::allocate(copy.length)), length(copy.length)
{
	Memory::copy(this->address, copy.address, this->length);
}

Memory::string::string(Memory::string &&move): address(move.address), length(move.length)
{
	move.address = 0;
	move.length = 0;
}

Memory::string::~string()
{
	Memory::free(this->address);
	this->address = NULL;
	this->length = 0;
}

Memory::string &Memory::string::operator=(const Memory::string &copy)
{
	if (&copy != this)
	{
		(*this) = Memory::string(copy);
	}
	return *this;
}

Memory::string &Memory::string::operator=(Memory::string &&move)
{
	if (&move != this)
	{
		Memory::free(this->address);
		this->address = move.address;
		this->length = move.length;
		move.address = 0;
		move.length = 0;
	}
	return *this;
}

BYTE &Memory::string::operator[](QWORD off) const
{
	return ((BYTE *)this->address)[off];
}

void Memory::string::ensure(QWORD size)
{
	if (this->length < size)
	{
		this->address = Memory::reallocate(this->address, size);
		this->length = size;
	}
}
