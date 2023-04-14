#include "definitions.h"

bool CWS::verify(const Memory::string &key, const Memory::string &accept)
{
	Memory::string expected = SecurityKey(key);
	if (expected.length == accept.length)
	{
		return Memory::compare(expected.address, accept.address, expected.length);
	}
	return false;
}