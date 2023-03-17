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
	class NibbleReader
	{
		public:
		Memory::string stream;
		QWORD position = 0;

		NibbleReader() = delete;
		COMPRESSIONAPI
		NibbleReader(Memory::string);
		COMPRESSIONAPI
		QWORD read(BYTE);
		COMPRESSIONAPI
		BYTE boundary();
		COMPRESSIONAPI
		void seek(QWORD);
		COMPRESSIONAPI
		void skip(QWORD);
	};

	namespace Brotli
	{
		COMPRESSIONAPI
		Memory::string encoding(const Memory::string &);
		COMPRESSIONAPI
		Memory::string decoding(const Memory::string &);
	}
}

#endif //COMPRESSION_H