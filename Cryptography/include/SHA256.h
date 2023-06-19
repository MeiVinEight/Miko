#ifndef SHA256_H
#define SHA256_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class SHA256: public MessageDigest
	{
		public:
		CRYPTOAPI SHA256();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //SHA256_H
