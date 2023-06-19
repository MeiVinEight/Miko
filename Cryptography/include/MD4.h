#ifndef MD4_H
#define MD4_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class MD4: public MessageDigest
	{
		public:
		CRYPTOAPI MD4();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //MD4_H
