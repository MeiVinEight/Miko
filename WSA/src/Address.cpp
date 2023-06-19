#include <endian.h>
#include <exception.h>
#include <Address.h>
#include <wsa.h>

#include "ws2_32.h"
#include "sockaddr.h"

extern "C"
{

int __stdcall getaddrinfo(const char *, const char *, const void *, void *);
void __stdcall freeaddrinfo(void *);
const char *__stdcall inet_ntop(int, const void *, char *, QWORD);
QWORD strlen(const char *);
#pragma intrinsic(strlen)

}

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
	int family;
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

	ret.resize(strlen(buf));
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
WSA::Address WSA::IP(const char *host)
{
	ADDRINFOA *info = nullptr;
	int err = getaddrinfo(host, nullptr, nullptr, &info);
	if (!err)
	{
		if (info)
		{
			WSA::Address address;
			switch (info->ai_family)
			{
				case WSA::AF_INET:
				{
					SOCKADDR_IN *addr = (SOCKADDR_IN *) info->ai_addr;
					IN_ADDR *ia = &addr->sin_addr;
					address.take(htonl(ia->S_un.S_addr));
					break;
				}
				case WSA::AF_INET6:
				{
					SOCKADDR_IN6 *addr = (SOCKADDR_IN6 *) info->ai_addr;
					IN6_ADDR *ia = &addr->sin6_addr;
					Memory::copy(address.address, ia, 16);
					break;
				}
			}
			freeaddrinfo(info);
			return address;
		}
		throw Memory::exception(WSA::ERRNO_UNKNOWN_HOST);
	}
	freeaddrinfo(info);
	throw Memory::exception(err, Memory::DOSERROR);
}