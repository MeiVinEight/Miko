#include <cws.h>
#include <SHA1.h>
#include <base64.h>

Memory::string CWS::security(const Memory::string &key)
{
	static const char GUID[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	static const DWORD appendSize = sizeof(GUID) - 1;

	Memory::string key1(key.length + appendSize);
	Memory::copy(key1.address, key.address, key.length);
	Memory::copy(key1.address + key.length, GUID, appendSize);

	Cryptography::SHA1 digest;
	digest.update(key1.address, key1.length);
	return Cryptography::BASE64::encode(digest.finally());
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