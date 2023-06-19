#include "CommonMessageDigest.h"

bool Appendix32(QWORD length, Memory::string &block, QWORD &position, void (*save)(QWORD, void *, BYTE))
{
	if (block.length - position < 8)
	{
		Memory::fill(block.address + position, 0, block.length - position);
		position = 0;
		return true;
	}
	Memory::fill(block.address + position, 0, block.length - 8 - position);
	position = block.length - 8;
	save(length, block.address + position, 8);
	position += 8;
	return false;
}
bool Appendix64(QWORD length, Memory::string &block, QWORD &position, void (*save)(QWORD, void *, BYTE))
{
	if (block.length - position < 16)
	{
		Memory::fill(block.address + position, 0, block.length - position);
		position = 0;
		return true;
	}
	Memory::fill(block.address + position, 0, block.length - 16 - position);
	position = block.length - 16;
	save(0, block.address + position, 8);
	position += 8;
	save(length, block.address + position, 8);
	position += 16;
	return false;
}
void Transform32(Memory::string &digest, void (*save)(QWORD, void *, BYTE))
{
	DWORD *buf = (DWORD *) digest.address;
	for (QWORD i = 0; i < digest.length >> 2; i++)
	{
		save(buf[i], buf + i, 4);
	}
}
void Transform64(Memory::string &digest, void (*save)(QWORD, void *, BYTE))
{
	QWORD *buf = (QWORD *) digest.address;
	for (QWORD i = 0; i < digest.length >> 3; i++)
	{
		save(buf[i], buf + i, 8);
	}
}