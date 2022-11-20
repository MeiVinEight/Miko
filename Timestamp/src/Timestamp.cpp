#include "timedef.h"

QWORD Timestamp::current()
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	QWORD time = ((QWORD)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	return (time / 10000) - offset;
}