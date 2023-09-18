#include <SocketAddress.h>

WSA::SocketAddress::SocketAddress() = default;
WSA::SocketAddress::SocketAddress(BYTE s0, BYTE s1, BYTE s2, BYTE s3, WORD port): IP(s0, s1, s2, s3), ID(port)
{
}
WSA::SocketAddress::SocketAddress(DWORD ip, WORD port): IP(ip), ID(port)
{
}
WSA::SocketAddress::SocketAddress(QWORD s0, QWORD s1, WORD port): IP(s0, s1), ID(port)
{
}
WSA::SocketAddress::SocketAddress(WORD port): IP(), ID(port)
{
}
WSA::SocketAddress::SocketAddress(const WSA::Address &ip, WORD port): IP(ip), ID(port)
{
}
Memory::string WSA::SocketAddress::stringify() const
{
	Memory::string ip = this->IP.string();
	BYTE port[5];
	BYTE off = 4;
	DWORD dwPort = this->ID;
	while (dwPort)
	{
		port[off--] = ('0' + (dwPort % 10));
		dwPort /= 10;
	}
	if (off == 4)
	{
		port[off--] = '0';
	}
	off++;
	Memory::string ret(ip.length + (2 * !this->IP.IPV4()) + 1 + (5 - off));
	QWORD idx = !this->IP.IPV4();
	Memory::copy(ret.address + idx, ip.address, ip.length);
	idx += idx;
	idx += ip.length;
	ret[idx] = ':';
	idx++;
	Memory::copy(ret.address + idx, port + off, 5 - off);
	if (!this->IP.IPV4())
	{
		ret[0] = '[';
		ret[1 + ip.length] = ']';
	}
	return ret;
}