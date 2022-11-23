#include "definitions.h"

DWORD WSA::Address::make() const
{
	return (this->address[0] << 24) | (this->address[1] << 16) | (this->address[2] << 8) | (this->address[3]);
}

void WSA::Address::take(DWORD addr)
{
	this->address[0] = (addr >> 24) & 0xFF;
	this->address[1] = (addr >> 16) & 0xFF;
	this->address[2] = (addr >> 8) & 0xFF;
	this->address[3] = addr & 0xFF;
}

Memory::string WSA::Address::string() const
{
	IN_ADDR ia;
	ia.S_un.S_un_b.s_b1 = this->address[0];
	ia.S_un.S_un_b.s_b2 = this->address[1];
	ia.S_un.S_un_b.s_b3 = this->address[2];
	ia.S_un.S_un_b.s_b4 = this->address[3];
	char *s = inet_ntoa(ia);
	if (s)
	{
		Memory::string ret(strlen(s) + 1);
		Memory::copy(ret, s, ret.length);
		return ret;
	}
	throw Exception::exception(Exception::message(WSAGetLastError()));
}