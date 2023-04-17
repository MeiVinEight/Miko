#include "definitions.h"

void Streaming::CR(Streaming::stream *stream)
{
	static char cr = '\r';
	stream->write(&cr, 1);
}
void Streaming::LF(Streaming::stream *stream)
{
	static char lf = '\n';
	stream->write(&lf, 1);
}
void Streaming::CRLF(Streaming::stream *stream)
{
	static char buf[2] = {'\r', '\n'};
	stream->write(buf, 2);
}