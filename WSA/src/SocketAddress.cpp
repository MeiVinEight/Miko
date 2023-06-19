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