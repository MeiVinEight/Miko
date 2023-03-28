#include "SHA512.h"

Crypto::SHA384::SHA384(): MessageDigest(Crypto::MessageDigest::BLOCK_SIZE_64, 64, &CalculateSHA512)
{
	QWORD H[8] = {
		0xcbbb9d5dc1059ed8,
		0x629a292a367cd507,
		0x9159015a3070dd17,
		0x152fecd8f70e5939,
		0x67332667ffc00b31,
		0x8eb44a8768581511,
		0xdb0c2e0d64f98fa7,
		0x47b5481dbefa4fa4
	};
	Memory::copy(this->digest.address, H, this->digest.length);
}
bool Crypto::SHA384::appendix(Memory::string &block, QWORD &position)
{
	return Appendix64(this->length, block, position, &SaveAsBEndian);
}
void Crypto::SHA384::transform(Memory::string &digest)
{
	Transform64(digest, &SaveAsBEndian);
	digest.resize(48);
}