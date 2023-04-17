#include "definitions.h"

Memory::string CWS::security(const Memory::string &key)
{
	return SecurityKey(key);
}
bool CWS::verify(const Memory::string &key, const Memory::string &accept)
{
	Memory::string expected = security(key);
	if (expected.length == accept.length)
	{
		return Memory::compare(expected.address, accept.address, expected.length);
	}
	return false;
}