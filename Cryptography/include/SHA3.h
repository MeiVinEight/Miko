#ifndef SHA3_H
#define SHA3_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	// TODO SHA512/224 SHA512/256
	class SHA3: public MessageDigest
	{
		private:
		const BYTE function;
		public:
		static const BYTE SHA224 = 0;
		static const BYTE SHA256 = 1;
		static const BYTE SHA384 = 2;
		static const BYTE SHA512 = 3;
		CRYPTOAPI SHA3(BYTE);
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
		CRYPTOAPI void update(const void *, QWORD) override;
		CRYPTOAPI Memory::string finally() override;
	};
}

#endif //SHA3_H
