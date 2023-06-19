#ifndef RNG_H
#define RNG_H

#include <WinType.h>
#include <CryptographySpec.h>

namespace Cryptography
{
	class RNG
	{
		public:
		CRYPTOAPI virtual ~RNG() noexcept;
		CRYPTOAPI virtual void seed(QWORD) = 0;
		CRYPTOAPI virtual QWORD random() = 0;
		/**
		* @param max
		*
		* Generate random number in [0, max - 1].
		* <br/>
		* Assume <code>r</code> is a random number in [0, 2<sup>64</sup>-1],
		* this works looks like: <code>r * (max/2<sup>64</sup>)</code>.
		* <br/>
		* Make x as a 128-bit unsigned number, and x = r * max,
		* finally, return x's highest 64 bits.
		*/
		CRYPTOAPI virtual QWORD random(QWORD);
	};
}

#endif //RNG_H
