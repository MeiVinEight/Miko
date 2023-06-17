#ifndef MIKO_NIBBLEREADER_H
#define MIKO_NIBBLEREADER_H

#include <CompressionSpec.h>
#include <WinType.h>
#include <memory.h>

namespace Compression
{
	class NibbleReader
	{
		public:
		Memory::string stream;
		QWORD position = 0;

		NibbleReader() = delete;
		COMPRESSIONAPI NibbleReader(Memory::string);
		COMPRESSIONAPI QWORD read(BYTE);
		COMPRESSIONAPI BYTE boundary();
		COMPRESSIONAPI void seek(QWORD);
		COMPRESSIONAPI void skip(QWORD);
	};
}

#endif //MIKO_NIBBLEREADER_H
