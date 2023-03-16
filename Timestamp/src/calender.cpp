#include "definitions.h"

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