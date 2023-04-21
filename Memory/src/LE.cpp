#include "definitions.h"

QWORD Memory::LE::get(const void *p, BYTE n)
{
	return GetAsLEndian(p, n);
}
WORD Memory::LE::get(WORD x)
{
	return Memory::LE::get(&x, 2);
}
DWORD Memory::LE::get(DWORD x)
{
	return Memory::LE::get(&x, 4);
}
QWORD Memory::LE::get(QWORD x)
{
	return Memory::LE::get(&x, 8);
}
void Memory::LE::set(QWORD x, void *p, BYTE n)
{
	return SaveAsLEndian(x, p, n);
}