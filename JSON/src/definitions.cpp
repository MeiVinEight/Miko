#include "definitions.h"

void Capacity(BYTE *(&buffer), QWORD &size, QWORD expect)
{
	if (expect > size)
	{
		BYTE *nbuf = new BYTE[2 * size];
		Memory::copy(nbuf, buffer, size);
		delete[] buffer;
		buffer = nbuf;
		size *= 2;
	}
}