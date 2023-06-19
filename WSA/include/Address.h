#ifndef ADDRESS_H
#define ADDRESS_H

#include <WSASpec.h>
#include <WinType.h>
#include <memory.h>

namespace WSA
{
	class Address
	{
		public:
		BYTE address[16]{0};

		WSAAPI Address();
		WSAAPI Address(BYTE, BYTE, BYTE, BYTE);
		WSAAPI Address(DWORD);
		WSAAPI Address(QWORD, QWORD);
		WSAAPI DWORD make() const;
		WSAAPI void take(DWORD);
		WSAAPI Memory::string string(bool) const;
		WSAAPI Memory::string string() const;
		WSAAPI bool IPV4() const;
	};
	WSAAPI WSA::Address IP(const char *);
}

#endif //ADDRESS_H
