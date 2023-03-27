#include "definitions.h"

void Streaming::LF(Streaming::stream *stream)
{
	static char lf = '\n';
	stream->write(&lf, 1);
}