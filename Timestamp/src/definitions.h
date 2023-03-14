#ifndef TIMEDEF_H
#define TIMEDEF_H

#define TIMESTAMP_SHARED
#include <timestamp.h>
#include <exception.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef struct
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

void GetSystemTimeAsFileTime(LPFILETIME);
BOOL FileTimeToSystemTime(const FILETIME *, LPSYSTEMTIME);
BOOL FileTimeToLocalFileTime(const FILETIME *, LPFILETIME);
void GetLocalTime(LPSYSTEMTIME);
DWORD __stdcall GetLastError(void);
void SetCalenderZone(LPSYSTEMTIME, Timestamp::calender *);


extern QWORD offset;


#ifdef __cplusplus
}
#endif

#endif //TIMEDEF_H
