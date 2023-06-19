#ifndef SHA384_H
#define SHA384_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class SHA384: public MessageDigest
	{
		public:
		CRYPTOAPI SHA384();
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
	};
}

#endif //SHA384_H
