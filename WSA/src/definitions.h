#ifndef WSADEF_H
#define WSADEF_H

#pragma comment(lib, "ws2_32.lib") // import ws2_32.lib

#define WSA_SHARED

#include <wsa.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define WSADESCRIPTION_LEN	256
#define WSASYS_STATUS_LEN	128
#define SOCKET_ERROR		(-1)
#define AF_INET				2               /* internetwork: UDP, TCP, etc. */
#define SOCK_STREAM			1               /* stream socket */
#define IPPROTO_TCP			6               /* tcp */
#define WSAECONNABORTED		10053L
#define FIONREAD			(0x40000000 | (((long) sizeof(u_long) & 0x7f) << 16) | (('f') << 8) | (127))
#define DLL_PROCESS_DETACH	0
#define DLL_PROCESS_ATTACH	1

typedef void 				*HANDLE, *HINSTANCE, *LPVOID;
typedef unsigned long		u_long;
typedef BYTE 				UCHAR;
typedef unsigned short		USHORT, u_short;
typedef DWORD				ULONG;
typedef unsigned __int64	size_t;
typedef int					INT;
typedef const char			*PCSTR;
typedef struct
{
	WORD					wVersion;
	WORD					wHighVersion;
	unsigned short			iMaxSockets;
	unsigned short			iMaxUdpDg;
	char *					lpVendorInfo;
	char					szDescription[WSADESCRIPTION_LEN+1];
	char					szSystemStatus[WSASYS_STATUS_LEN+1];
} WSADATA, *LPWSADATA;
typedef struct
{
	char    * h_name;           /* official name of host */
	char    ** h_aliases;  /* alias list */
	short   h_addrtype;             /* host address type */
	short   h_length;               /* length of address */
	char    ** h_addr_list; /* list of addresses */
} HOSTENT;
typedef struct
{
	union
	{
		struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { USHORT s_w1,s_w2; } S_un_w;
		ULONG S_addr;
	} S_un;
} IN_ADDR;
typedef struct
{
	short   sin_family;
	u_short sin_port;
	IN_ADDR sin_addr;
	char    sin_zero[8];
} SOCKADDR_IN;
typedef struct
{
	u_short sa_family;              /* address family */
	char    sa_data[14];            /* up to 14 bytes of direct address */
} SOCKADDR;
typedef struct addrinfo
{
	int			ai_flags;
	int			ai_family;
	int			ai_socktype;
	int			ai_protocol;
	size_t		ai_addrlen;
	char 		*ai_canonname;
	SOCKADDR 	*ai_addr;
	addrinfo	*ai_next;
} ADDRINFOA, *PADDRINFOA;

int __stdcall WSAStartup(WORD, LPWSADATA);
int __stdcall WSACleanup(void);
int __stdcall WSAGetLastError(void);
HOSTENT *__stdcall gethostbyname(const char *);
SOCKET __stdcall socket(int, int, int);
u_long __stdcall htonl(u_long);
u_short __stdcall htons(u_short);
int __stdcall bind(SOCKET, const SOCKADDR*, int);
int __stdcall listen(SOCKET, int);
SOCKET __stdcall accept(SOCKET, SOCKADDR*, int *);
int __stdcall closesocket(SOCKET);
int __stdcall connect(SOCKET, const SOCKADDR *, int);
int __stdcall recv(SOCKET, char *, int, int);
int __stdcall send(SOCKET, const char *, int, int);
int __stdcall ioctlsocket(SOCKET, long, u_long *);
int __stdcall getsockname(SOCKET, SOCKADDR *, int *);
INT __stdcall getaddrinfo(PCSTR, PCSTR, const ADDRINFOA *, PADDRINFOA *);
void __stdcall freeaddrinfo(PADDRINFOA);
char *__stdcall inet_ntoa(IN_ADDR);


#ifdef __cplusplus
}
#endif

QWORD strlen(const void *);

#endif //WSADEF_H
