#include "SHA256.h"

Crypto::SHA224::SHA224(): MessageDigest(Crypto::MessageDigest::BLOCK_SIZE_32, 32, &CalculateSHA256)
{
	DWORD H[8] = {0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939, 0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4};
	Memory::copy(this->digest.address, H, this->digest.length);
}
bool Crypto::SHA224::appendix(Memory::string &block, QWORD &position)
{
	return Appendix32(this->length, block, position, &SaveAsBEndian);
}
void Crypto::SHA224::transform(Memory::string &digest)
{
	Transform32(digest, &SaveAsBEndian);
	digest.resize(28);
}
