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
		DWORD read(void *, DWORD) override;
		DWORD write(const void *, DWORD) override;
		void flush() override;
		QWORD available() override;
	};
}

#endif //STRINGSTREAM_H
