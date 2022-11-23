#include "definitions.h"

QWORD offset = 11644473600000ULL;

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