#ifndef SHA224_H
#define SHA224_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class SHA224: public MessageDigest
	{
		public:
		CRYPTOAPI SHA224();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //SHA224_H
