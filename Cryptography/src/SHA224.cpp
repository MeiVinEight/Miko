#include <endian.h>
#include <SHA224.h>

#include "SHA256C.h"
#include "CommonMessageDigest.h"

Cryptography::SHA224::SHA224(): MessageDigest(Cryptography::MessageDigest::BLOCK_SIZE_32, 32, &CalculateSHA256)
{
	DWORD H[8] = {0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939, 0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4};
	Memory::copy(this->digest.address, H, this->digest.length);
}
bool Cryptography::SHA224::appendix(Memory::string &block, QWORD &position)
{
	return Appendix32(this->length, block, position, &Memory::BE::set);
}
void Cryptography::SHA224::transform(Memory::string &digest)
{
	Transform32(digest, &Memory::BE::set);
	digest.resize(28);
}
