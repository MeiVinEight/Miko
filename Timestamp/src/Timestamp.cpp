#include <timestamp.h>

#include "definitions.h"

extern "C"
{
void GetSystemTimeAsFileTime(void *);
}

typedef struct
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

QWORD Timestamp::current()
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	QWORD time = ((QWORD)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	return (time / 10000) - offset;
}