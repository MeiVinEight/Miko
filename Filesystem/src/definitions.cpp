#include "definitions.h"

QWORD strlen(const void *str)
{
	const char *s = (const char *) str;
	while (*s++);
	return s - ((const char *)str) - 1;
}
Memory::string cstring(const String::string &str)
{
	Memory::string cstr(str.length() + 1);
	Memory::copy(cstr.address, str.address.address, str.length());
	cstr[str.length()] = 0;
	return cstr;
}