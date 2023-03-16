#include "definitions.h"

Streaming::format Streaming::cin(Filesystem::STDIN);
Streaming::format Streaming::cout(Filesystem::STDOUT);

void Streaming::LF(Streaming::stream *stream)
{
	static char lf = '\n';
	stream->write(&lf, 1);
}