#ifndef CALENDER_H
#define CALENDER_H

#include <TimestampSpec.h>
#include <WinType.h>

namespace Timestamp
{

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

		TIMEAPI void convert();
		TIMEAPI void convert(QWORD);
		TIMEAPI WORD &operator[](WORD);
	};
}

#endif //CALENDER_H
