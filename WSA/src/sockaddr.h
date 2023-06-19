#ifndef SOCKADDR_H
#define SOCKADDR_H

#include <WinType.h>

typedef struct
{
	union
	{
		struct
		{
			BYTE s_b1;
			BYTE s_b2;
			BYTE s_b3;
			BYTE s_b4;
		} S_un_b;
		struct
		{
			WORD s_w1;
			WORD s_w2;
		} S_un_w;
		DWORD S_addr;
	} S_un;
} IN_ADDR;
typedef struct
{
	union
	{
		BYTE Byte[16];
		WORD Word[8];
	} u;
} IN6_ADDR, *PIN6_ADDR, *LPIN6_ADDR;
typedef struct
{
	WORD sa_family;              /* address family */
	char    sa_data[14];            /* up to 14 bytes of direct address */
} SOCKADDR;
typedef struct
{
	short   sin_family;
	WORD sin_port;
	IN_ADDR sin_addr;
	char    sin_zero[8];
} SOCKADDR_IN;
typedef struct
{
	union
	{
		struct
		{
			DWORD Zone;
			DWORD Level;
		};
		DWORD Value;
	};
} SCOPE_ID, *PSCOPE_ID;
typedef struct
{
	WORD sin6_family; // AF_INET6.
	WORD sin6_port;           // Transport level port number.
	DWORD sin6_flowinfo;       // IPv6 flow information.
	IN6_ADDR sin6_addr;         // IPv6 address.
	union
	{
		DWORD sin6_scope_id;     // Set of interfaces for a scope.
		SCOPE_ID sin6_scope_struct;
	};
} SOCKADDR_IN6, *PSOCKADDR_IN6, *LPSOCKADDR_IN6;
typedef struct addrinfo
{
	int             ai_flags;
	int             ai_family;
	int             ai_socktype;
	int             ai_protocol;
	QWORD           ai_addrlen;
	char            *ai_canonname;
	SOCKADDR        *ai_addr;
	struct addrinfo *ai_next;
} ADDRINFOA, *PADDRINFOA;

#endif //SOCKADDR_H
