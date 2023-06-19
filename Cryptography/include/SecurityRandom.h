#ifndef SECURITYRANDOM_H
#define SECURITYRANDOM_H

#include <CryptographySpec.h>
#include <RNG.h>

namespace Cryptography
{
	class SecurityRandom: public RNG
	{
		public:
		CRYPTOAPI void seed(QWORD) override;
		CRYPTOAPI QWORD random() override;
		CRYPTOAPI QWORD random(QWORD) override;
	};
}

#endif //SECURITYRANDOM_H
