#include <timestamp.h>

#include "definitions.h"

extern "C"
{
void GetSystemTimeAsFileTime(void *);
BOOL FileTimeToLocalFileTime(const void *, void *);
}

typedef struct
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

QWORD Timestamp::current()
{
	FILETIME ft, lft;
	GetSystemTimeAsFileTime(&ft);
	if (FileTimeToLocalFileTime(&ft, &lft))
	{
		QWORD &time = *((QWORD *) &lft);
		return (time /= 10000) -= offset;
	}
	// throw Memory::exception(Memory::DOSERROR, Memory::error());
	return ~((QWORD) 0);
}