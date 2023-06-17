#ifndef STANDARDSTREAM_H
#define STANDARDSTREAM_H

#include <filestream.h>
#include <format.h>

namespace Streaming
{
	FSAPI extern Streaming::file input;
	FSAPI extern Streaming::file output;
	FSAPI extern Streaming::format cin;
	FSAPI extern Streaming::format cout;
}

#endif //STANDARDSTREAM_H
