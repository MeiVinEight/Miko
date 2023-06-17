#include <calender.h>

#include "definitions.h"

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

extern "C"
{
BOOL FileTimeToSystemTime(const void *, void *);
BOOL FileTimeToLocalFileTime(const void *, void *);
void GetLocalTime(void *);
}

void SetCalenderZone(LPSYSTEMTIME systemTime, Timestamp::calender *pCal)
{
	Timestamp::calender &cal = *pCal;
	cal[Timestamp::calender::YEAR] = systemTime->wYear;
	cal[Timestamp::calender::MONTH] = systemTime->wMonth;
	cal[Timestamp::calender::DAY_OF_WEEK] = ((systemTime->wDayOfWeek + 6) % 7) + 1;
	cal[Timestamp::calender::DAY] = systemTime->wDay;
	cal[Timestamp::calender::HOUR] = systemTime->wHour;
	cal[Timestamp::calender::MINUTE] = systemTime->wMinute;
	cal[Timestamp::calender::SECOND] = systemTime->wSecond;
	cal[Timestamp::calender::MILLISECOND] = systemTime->wMilliseconds;
}

void Timestamp::calender::convert()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	SetCalenderZone(&st, this);
}
void Timestamp::calender::convert(QWORD timestamp)
{
	timestamp += offset;
	timestamp *= 10000;
	FILETIME ft;
	ft.dwLowDateTime = (timestamp >> 0x00) & 0xFFFFFFFF;
	ft.dwHighDateTime = (timestamp >> 0x20) & 0xFFFFFFFF;
	FILETIME lft;
	SYSTEMTIME st;
	if (FileTimeToLocalFileTime(&ft, &lft))
	{
		if (FileTimeToSystemTime(&lft, &st))
		{
			SetCalenderZone(&st, this);
		}
	}
	// DWORD x = GetLastError();
	// throw Exception::exception(Exception::message(x));
}
WORD &Timestamp::calender::operator[](WORD idx)
{
	return this->zone[idx];
}