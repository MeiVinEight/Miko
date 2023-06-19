#ifndef MESSAGEDIGEST_H
#define MESSAGEDIGEST_H

#include <CryptographySpec.h>
#include <memory.h>

namespace Cryptography
{
	class MessageDigest
	{
		protected:
		Memory::string block;
		QWORD position = 0;
		Memory::string digest;
		QWORD length = 0;
		void (*computation)(BYTE *, BYTE *);
		public:
		static const DWORD BLOCK_SIZE_32 = 0x40;
		static const DWORD BLOCK_SIZE_64 = 0x80;
		CRYPTOAPI MessageDigest(QWORD, QWORD, void (*)(BYTE *, BYTE *));
		CRYPTOAPI virtual ~MessageDigest() noexcept;
		CRYPTOAPI virtual bool appendix(Memory::string &, QWORD &) = 0;
		CRYPTOAPI virtual void transform(Memory::string &) = 0;
		CRYPTOAPI virtual void update(const void *, QWORD);
		CRYPTOAPI virtual Memory::string finally();
	};
}

#endif //MESSAGEDIGEST_H
