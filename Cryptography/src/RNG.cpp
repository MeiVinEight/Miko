#include <RNG.h>

Cryptography::RNG::~RNG() noexcept = default;
QWORD Cryptography::RNG::random(QWORD max)
{
	QWORD rh = 0;
	QWORD rl = 0;
	QWORD ah = 0;
	QWORD al = this->random();

	while (max)
	{
		if (max & 1)
		{
			QWORD h = ah;
			QWORD l = al;
			while (h || l)
			{
				QWORD ih = rh & h;
				QWORD il = rl & l;
				rh ^= h;
				rl ^= l;
				l = il << 1;
				h = ih << 1 | (il >> 63);
			}
		}

		max >>= 1;
		ah <<= 1;
		ah |= (al >> 63);
		al <<= 1;
	}
	return rh;
}