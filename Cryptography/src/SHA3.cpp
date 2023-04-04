#include "Keccak.h"

QWORD SHA3FUNCTIONS[4] = {224, 256, 384, 512};

Cryptography::SHA3::SHA3(BYTE type): Cryptography::MessageDigest::MessageDigest(200, 200, nullptr), function(type)
{
	Memory::fill(this->block.address, 0, this->block.length);
	Memory::fill(this->digest.address, 0, this->digest.length);
}
bool Cryptography::SHA3::appendix(Memory::string &, QWORD &)
{
	return false;
}
void Cryptography::SHA3::transform(Memory::string &)
{
}
void Cryptography::SHA3::update(const void *data, QWORD len)
{
	QWORD b = 1600;
	QWORD c = SHA3FUNCTIONS[this->function] << 1;
	QWORD size = (b - c) >> 3;
	this->length += len << 3;
	const char *buf = (const char *) data;
	while (len)
	{
		QWORD copy = size - this->position;
		copy = len < copy ? len : copy;
		Memory::copy(this->block.address + this->position, buf, copy);
		buf += copy;
		this->position += copy;
		len -= copy;
		if (this->position >= size)
		{
			this->position = 0;
			SPONGE(this->digest.address, this->block.address);
		}
	}
}
Memory::string Cryptography::SHA3::finally()
{
	QWORD b = 1600;
	QWORD c = SHA3FUNCTIONS[this->function] << 1;
	QWORD size = (b - c) >> 3;
	Memory::string blk = this->block;
	Memory::string out = this->digest;
	QWORD pos = this->position;
	blk[pos++] = 6;
	PAD101(blk.address, size, pos);
	SPONGE(out.address, blk.address);
	out.resize(SHA3FUNCTIONS[this->function] >> 3);
	return out;
}