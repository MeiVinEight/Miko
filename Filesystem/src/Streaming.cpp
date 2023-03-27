#include "definitions.h"

Streaming::file Streaming::input(Filesystem::STDIN);
Streaming::file Streaming::output(Filesystem::STDOUT);
Streaming::format Streaming::cin(&Streaming::input);
Streaming::format Streaming::cout(&Streaming::output);