#ifndef MD5_H
#define MD5_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class MD5: public MessageDigest
	{
		public:
		CRYPTOAPI MD5();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //MD5_H
