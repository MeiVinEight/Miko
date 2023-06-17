#include <endian.h>

#include "CommonEndian.h"

QWORD Memory::BE::get(const void *p, BYTE n)
{
	return GetAsBEndian(p, n);
}
WORD Memory::BE::get(WORD x)
{
	return Memory::BE::get(&x, 2);
}
DWORD Memory::BE::get(DWORD x)
{
	return Memory::BE::get(&x, 4);
}
QWORD Memory::BE::get(QWORD x)
{
	return Memory::BE::get(&x, 8);
}
void Memory::BE::set(QWORD x, void *p, BYTE n)
{
	return SaveAsBEndian(x, p, n);
}
