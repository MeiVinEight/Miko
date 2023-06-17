#ifndef STREAMING_H
#define STREAMING_H

#include <streamingapi.h>
#include <stream.h>

namespace Streaming
{
	STREAMINGAPI void CR(Streaming::stream *);
	STREAMINGAPI void LF(Streaming::stream *);
	STREAMINGAPI void CRLF(Streaming::stream *);
}

#endif //STREAMING_H
