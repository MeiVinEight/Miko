#include "wsadef.h"

QWORD strlen(const void *str)
{
	const char *s = (const char *) str;
	while (*s++);
	return s - ((const char *)str) - 1;
}