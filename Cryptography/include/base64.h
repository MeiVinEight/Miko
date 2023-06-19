#ifndef BASE64_H
#define BASE64_H

#include <CryptographySpec.h>
#include <memory.h>

namespace Cryptography
{
	namespace BASE64
	{
		CRYPTOAPI Memory::string encode(const Memory::string &);
		CRYPTOAPI Memory::string decode(const Memory::string &);
	}
}

#endif //BASE64_H
