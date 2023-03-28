#include "definitions.h"

Cryptography::MessageDigest::MessageDigest(QWORD block, QWORD digest, void (*computation)(BYTE *, BYTE *)):
block(block), digest(digest), computation(computation)
{
}
void Cryptography::MessageDigest::update(const void *data, QWORD len)
{
	this->length += len << 3;
	const char *buf = (const char *) data;
	while (len)
	{
		QWORD copy = this->block.length - this->position;
		copy = len < copy ? len : copy;
		Memory::copy(this->block.address + this->position, buf, copy);
		buf += copy;
		this->position += copy;
		len -= copy;
		if (this->position >= this->block.length)
		{
			this->position = 0;
			this->computation(this->block.address, this->digest.address);
		}
	}
}
Memory::string Cryptography::MessageDigest::finally()
{
	Memory::string dig = this->digest;
	Memory::string blk = this->block;
	QWORD pos = this->position;
	blk[pos++] = 0x80;
	bool append;
	do
	{
		append = this->appendix(blk, pos);
		this->computation(blk.address, dig.address);
	}
	while (append);
	this->transform(dig);
	return dig;
}