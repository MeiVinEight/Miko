#include "memdef.h"

void *Memory::allocate(QWORD size)
{
	size = size ? size : 1;
	return HeapAlloc(heap, 0, size);
}

void *Memory::reallocate(void *p, QWORD size)
{
	if (p)
	{
		if (!size)
		{
			Memory::free(p);
			return NULL;
		}
		return HeapReAlloc(heap, 0, p, size);
	}
	return Memory::allocate(size);
}

void Memory::free(void *p)
{
	if (p)
		HeapFree(heap, 0, p); // Return value unused
}

void Memory::copy(void *dst, const void *src, QWORD len)
{
	char *d = (char *)dst;
	char *s = (char *)src;
	for (QWORD i = 0; i++ < len; d[i - 1] = s[i - 1]);
}

void Memory::fill(void *p, BYTE x, QWORD size)
{
	char *s= (char *)p;
	for (QWORD i = 0; i < size; s[i++] = (char)x);
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