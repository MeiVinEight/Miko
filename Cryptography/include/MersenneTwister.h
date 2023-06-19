#ifndef MERSENNETWISTER_H
#define MERSENNETWISTER_H

#include <CryptographySpec.h>
#include <RNG.h>

namespace Cryptography
{
	/**
	* MT19937-64 engine, generate random number in [0, 2<sup>64</sup>-1].
	*/
	class MersenneTwister: public RNG
	{
		private:
		static const int W = 64;
		static const int N = 312;
		static const int M = 156;
		static const int R = 31;
		static const int U = 29;
		static const int S = 17;
		static const int T = 37;
		static const int L = 43;
		static const QWORD A = 0XB5026F5AA96619E9ULL;
		static const QWORD F = 0X5851F42D4C957F2DULL;
		static const QWORD D = 0X5555555555555555ULL;
		static const QWORD B = 0X71D67FFFEDA60000ULL;
		static const QWORD C = 0XFFF7EEE000000000ULL;
		static const QWORD LOWER_MASK = (1ULL << R) - 1;
		static const QWORD UPPER_MASK = (~LOWER_MASK);
		QWORD MT[Cryptography::MersenneTwister::N]{0};
		QWORD K = 0;
		public:
		CRYPTOAPI void seed(QWORD) override;
		CRYPTOAPI void twist();
		CRYPTOAPI QWORD random() override;
		CRYPTOAPI QWORD random(QWORD) override;
	};
}

#endif //MERSENNETWISTER_H
