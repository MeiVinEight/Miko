#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#ifdef TIMESTAMP_SHARED
	#define TIMEAPI __declspec(dllexport)
#else
	#define TIMEAPI
#endif

// TODO use common type definitions
typedef int					BOOL;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;

namespace Timestamp
{
	/**
	 * A file time is a 64-bit value that represents the number
	 * of 100-nanosecond intervals that have elapsed since
	 * 12:00 A.M. January 1, 1601 Coordinated Universal Time (UTC).
	 * <br/>
	 * Reference: <a href="https://learn.microsoft.com/en-us/windows/win32/sysinfo/file-times">File Times</a>
	 * <br/>
	 * This function will convert this value to millis since
	 * 00:00:00 A.M. January 1, 1970 (1970/01/01 00:00:00)
	 */
	TIMEAPI
	QWORD current();

	class calender
	{
		public:
		static const WORD YEAR			= 0;
		static const WORD MONTH			= 1;
		static const WORD DAY_OF_WEEK	= 2;
		static const WORD DAY			= 3;
		static const WORD HOUR			= 4;
		static const WORD MINUTE		= 5;
		static const WORD SECOND		= 6;
		static const WORD MILLISECOND	= 7;
		WORD zone[8]{0};

		TIMEAPI
		void convert();
		TIMEAPI
		void convert(QWORD);
		TIMEAPI
		WORD &operator[](WORD);
	};
}

#endif //TIMESTAMP_H
