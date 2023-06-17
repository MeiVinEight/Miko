#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <TimestampSpec.h>
#include <WinType.h>

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
	TIMEAPI QWORD current();
}

#endif //TIMESTAMP_H
