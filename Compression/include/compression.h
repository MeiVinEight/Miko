#ifndef COMPRESSION_H
#define COMPRESSION_H

#ifdef COMPRESSION_SHARED
	#define COMPRESSIONAPI __declspec(dllexport)
#else
	#define COMPRESSIONAPI
#endif

#include <memory.h>

namespace Compression
{
	namespace LZ77
	{
	}

	namespace Brotli
	{
		COMPRESSIONAPI
		Memory::string encoding(const Memory::string &);
		COMPRESSIONAPI
		Memory::string decoding(const Memory::string &);
	}
}

#endif //COMPRESSION_H