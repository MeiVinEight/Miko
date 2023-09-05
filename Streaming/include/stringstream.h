#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include <stream.h>

namespace Streaming
{
	class string: public Streaming::stream
	{
		public:
		Memory::string address;
		QWORD position = 0;
		STREAMINGAPI DWORD read(void *, DWORD) override;
		STREAMINGAPI DWORD write(const void *, DWORD) override;
		STREAMINGAPI void flush() override;
		STREAMINGAPI QWORD available() override;
	};
}

#endif //STRINGSTREAM_H
