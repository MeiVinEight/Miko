#ifndef CRYPTO_H
#define CRYPTO_H

#ifdef CRYPTO_SHARED
	#define CRYPTOAPI __declspec(dllexport)
#else
	#define CRYPTOAPI __declspec(dllimport)
#endif

#include <memory.h>

namespace Crypto
{
	namespace BASE64
	{
		CRYPTOAPI Memory::string encode(const Memory::string &);
		CRYPTOAPI Memory::string decode(const Memory::string &);
	}
}

#endif //CRYPTO_H