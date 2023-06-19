#ifndef SHAKE_H
#define SHAKE_H

#include <CryptographySpec.h>
#include <MessageDigest.h>

namespace Cryptography
{
	class SHAKE: public MessageDigest
	{
		private:
		const BYTE function;
		public:
		static const BYTE SHAKE128 = 0;
		static const BYTE SHAKE256 = 1;
		const QWORD size;
		CRYPTOAPI SHAKE(BYTE, QWORD);
		CRYPTOAPI bool appendix(Memory::string &, QWORD &) override;
		CRYPTOAPI void transform(Memory::string &) override;
		CRYPTOAPI void update(const void *, QWORD) override;
		CRYPTOAPI Memory::string finally() override;
	};
}

#endif //SHAKE_H
