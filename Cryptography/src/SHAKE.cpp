#include "Keccak.h"

const QWORD SHAKEFUNCTIONS[2] = {128, 256};

Cryptography::SHAKE::SHAKE(BYTE type, QWORD output): MessageDigest(200, 200, nullptr), function(type), size(output)
{
	Memory::fill(this->block.address, 0, this->block.length);
	Memory::fill(this->digest.address, 0, this->digest.length);
}
bool Cryptography::SHAKE::appendix(Memory::string &, QWORD &)
{
	return false;
}
void Cryptography::SHAKE::transform(Memory::string &)
{
}
void Cryptography::SHAKE::update(const void *data, QWORD len)
{
	QWORD b = 1600;
	QWORD c = SHAKEFUNCTIONS[this->function] << 1;
	QWORD blkSize = (b - c) >> 3;
	this->length += len << 3;
	const char *buf = (const char *) data;
	while (len)
	{
		QWORD copy = blkSize - this->position;
		copy = len < copy ? len : copy;
		Memory::copy(this->block.address + this->position, buf, copy);
		buf += copy;
		this->position += copy;
		len -= copy;
		if (this->position >= blkSize)
		{
			this->position = 0;
			SPONGE(this->digest.address, this->block.address);
		}
	}
}
Memory::string Cryptography::SHAKE::finally()
{
	Memory::string out((this->size >> 3) + !!(this->size & 7));
	if (this->size)
	{
		QWORD b = 1600;
		QWORD c = SHAKEFUNCTIONS[this->function] << 1;
		QWORD blkSize = (b - c) >> 3;
		Memory::string blk = this->block;
		Memory::string dig = this->digest;
		QWORD pos = this->position;
		blk[pos++] = 0x1F;
		PAD101(blk.address, blkSize, pos);
		SPONGE(dig.address, blk.address);
		QWORD offset = 0;
		while (offset < out.length)
		{
			QWORD copy = out.length - offset;
			copy = copy < blkSize ? copy : blkSize;
			Memory::copy(out.address + offset, dig.address, copy);
			offset += copy;
			KeccakF(dig.address);
		}
		BYTE msk = 0xFF;
		BYTE bit = (8 - (size % 8)) % 8;
		msk <<= bit;
		msk >>= bit;
		out[out.length - 1] &= msk;
	}
	return out;
}