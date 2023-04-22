#include "definitions.h"

WSA::Address::Address() = default;
WSA::Address::Address(BYTE s0, BYTE s1, BYTE s2, BYTE s3)
{
	DWORD s = 0;
	s |= (s0 << 0x18);
	s |= (s1 << 0x10);
	s |= (s2 << 0x08);
	s |= (s3 << 0x00);
	this->take(s);
}
WSA::Address::Address(DWORD s)
{
	this->take(s);
}
WSA::Address::Address(QWORD s0, QWORD s1)
{
	Memory::BE::set(s0, this->address + 0, 8);
	Memory::BE::set(s1, this->address + 8, 8);
}
DWORD WSA::Address::make() const
{
	return (this->address[12] << 24) | (this->address[13] << 16) | (this->address[14] << 8) | (this->address[15]);
}
void WSA::Address::take(DWORD addr)
{
	BYTE s0 = (addr >> 0x18) & 0xFF;
	BYTE s1 = (addr >> 0x10) & 0xFF;
	BYTE s2 = (addr >> 0x08) & 0xFF;
	BYTE s3 = (addr >> 0x00) & 0xFF;
	for (BYTE i = 0; i < 10; this->address[i++] = 0);
	this->address[10] = this->address[11] = 0xFF;
	this->address[12] = s0;
	this->address[13] = s1;
	this->address[14] = s2;
	this->address[15] = s3;
}
Memory::string WSA::Address::string(bool onlyV6) const
{
	char buf[64] = {};
	Memory::string ret;
	INT family;
	union
	{
		IN_ADDR V4 = {};
		IN6_ADDR V6;
	} ia;

	if (!onlyV6 && this->IPV4())
	{
		family = WSA::AF_INET;
		ia.V4.S_un.S_addr = htonl(this->make());
	}
	else
	{
		family = WSA::AF_INET6;
		Memory::copy(ia.V6.u.Byte, this->address, 16);
	}

	if (!inet_ntop(family, &ia, buf, 64))
	{
		throw Memory::exception(WSAGetLastError(), Memory::DOSERROR);
	}

	ret.resize(StringLength(buf));
	Memory::copy(ret.address, buf, ret.length);
	return ret;
}
Memory::string WSA::Address::string() const
{
	return this->string(false);
}
bool WSA::Address::IPV4() const
{
	WORD *addr = (WORD *) this->address;
	return addr[0] == addr[1] == addr[2] == addr[3] == addr[4] == 0 && addr[5] == 0xFFFF;
}