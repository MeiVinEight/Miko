#ifndef SHA512_H
#define SHA512_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class SHA512: public MessageDigest
	{
		public:
		CRYPTOAPI SHA512();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //SHA512_H
