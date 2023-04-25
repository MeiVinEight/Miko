#include "definitions.h"

QWORD StringLengthW(const void *str)
{
	WCHAR *s = (WCHAR *) str;
	QWORD len = 0;
	while (s[len++]);
	return len - 1;
}