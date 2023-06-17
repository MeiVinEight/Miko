#include "BinaryStorage.h"

int NumberSize(QWORD x)
{
	int v = 0;
	while (x)
	{
		v++;
		x >>= 1;
	}
	return v;
}
int LeadingZeros(QWORD x)
{
	return 64 - NumberSize(x);
}