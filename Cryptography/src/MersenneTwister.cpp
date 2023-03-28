#include "definitions.h"

void Cryptography::MersenneTwister::seed(QWORD seed)
{
	this->K = Cryptography::MersenneTwister::N;
	this->MT[0] = seed;
	for (int i = 1; i < Cryptography::MersenneTwister::N; i++)
	{
		MT[i] = (F * (MT[i - 1] ^ (MT[i - 1] >> (W - 2))) + i);
	}
}
void Cryptography::MersenneTwister::twist()
{
	for (int i = 0; i < Cryptography::MersenneTwister::N; i++)
	{
		QWORD x = (MT[i] & UPPER_MASK) + (MT[(i + 1) % Cryptography::MersenneTwister::N] & LOWER_MASK);
		QWORD xA = x >> 1;
		xA ^= ((x & 1) * Cryptography::MersenneTwister::A);
		MT[i] = MT[(i + Cryptography::MersenneTwister::M) % Cryptography::MersenneTwister::N] ^ xA;
	}
	this->K = 0;
}
QWORD Cryptography::MersenneTwister::random()
{
	if (this->K >= Cryptography::MersenneTwister::N)
	{
		this->twist();
	}

	QWORD x;
	QWORD y;
	QWORD z;
	x = MT[this->K];
	y = x ^ ((x >> Cryptography::MersenneTwister::U) & Cryptography::MersenneTwister::D);
	y = y ^ ((y << Cryptography::MersenneTwister::S) & Cryptography::MersenneTwister::B);
	y = y ^ ((y << Cryptography::MersenneTwister::T) & Cryptography::MersenneTwister::C);
	z = y ^ ((y >> Cryptography::MersenneTwister::L));

	this->K++;
	return z;
}
QWORD Cryptography::MersenneTwister::random(QWORD max)
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