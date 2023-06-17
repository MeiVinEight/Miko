#ifndef BROTLI_H
#define BROTLI_H

#include <CompressionSpec.h>
#include <memory.h>

namespace Compression
{

	namespace Brotli
	{
		COMPRESSIONAPI Memory::string encoding(const Memory::string &);
		COMPRESSIONAPI Memory::string decoding(const Memory::string &);
	}
}

#endif //BROTLI_H
