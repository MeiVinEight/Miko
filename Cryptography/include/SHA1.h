#ifndef SHA1_H
#define SHA1_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class SHA1: public MessageDigest
	{
		public:
		CRYPTOAPI SHA1();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //SHA1_H
