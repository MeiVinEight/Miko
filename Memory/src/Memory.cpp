#include <exception.h>

#include "crt.h"

DWORD order_value = 1;
char *order_point = (char *) &order_value;

const BYTE Memory::BENDIAN = order_point[0] == 0;
const BYTE Memory::LENDIAN = order_point[0] == 1;
const DWORD Memory::ERRNO_SUCCESS = Memory::registry("The operation completed successfully");
const DWORD Memory::ERRNO_ACCESS_VIOLATION = Memory::registry("Memory access violation");
const DWORD Memory::ERRNO_OBJECT_CLOSED = Memory::registry("Kernel object has been closed");

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
void Memory::reverse(void *b, QWORD len)
{
	BYTE *buf = (BYTE *) b;
	for (QWORD i = 0; i < len >> 1; i++)
	{
		BYTE t = buf[i];
		buf[i] = buf[len - i - 1];
		buf[len - i - 1] = t;
	}
}
void Memory::violation()
{
	*((BYTE *)1) = 0; // Access violation
}
