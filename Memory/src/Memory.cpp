#include "memdef.h"

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

void Memory::fill(void *p, BYTE x, QWORD size)
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