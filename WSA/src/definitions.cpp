#include "definitions.h"
#pragma comment(lib, "ws2_32.lib") // import ws2_32.lib

QWORD StringLength(const void *str)
{
	const char *s = (const char *) str;
	while (*s++);
	return s - ((const char *)str) - 1;
}