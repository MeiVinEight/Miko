#ifndef WSADEF_H
#define WSADEF_H

#define WSA_SHARED

#include <wsa.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define WSADESCRIPTION_LEN  256
#define WSASYS_STATUS_LEN   128
#define SOCKET_ERROR        (-1)
#define FIONREAD            (0x40000000 | (((long) sizeof(u_long) & 0x7f) << 16) | (('f') << 8) | (127))
#define DLL_PROCESS_DETACH  0
#define DLL_PROCESS_ATTACH  1
#define FD_SETSIZE          64

typedef void             *HANDLE, *HINSTANCE, *LPVOID;
typedef char             CHAR, *PSTR;
typedef int              INT;
typedef const char       *PCSTR;
typedef unsigned short   USHORT, u_short, ADDRESS_FAMILY;
typedef unsigned int     u_int;
typedef unsigned long    u_long;
typedef unsigned __int64 size_t;
typedef BYTE             UCHAR;
typedef DWORD            ULONG;
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
typedef struct fd_set
{
	u_int fd_count;               /* how many are SET? */
	SOCKET  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
} fd_set;
typedef struct timeval
{
	long tv_sec;         /* seconds */
	long tv_usec;        /* and microseconds */
} TIMEVAL;
typedef struct in6_addr
{
	union
	{
		UCHAR       Byte[16];
		USHORT      Word[8];
	} u;
} IN6_ADDR, *PIN6_ADDR, *LPIN6_ADDR;
typedef struct
{
	union
	{
		struct
		{
			ULONG Zone : 28;
			ULONG Level : 4;
		};
		ULONG Value;
	};
} SCOPE_ID, *PSCOPE_ID;
typedef struct sockaddr_in6
{
	ADDRESS_FAMILY sin6_family; // AF_INET6.
	USHORT sin6_port;           // Transport level port number.
	ULONG  sin6_flowinfo;       // IPv6 flow information.
	IN6_ADDR sin6_addr;         // IPv6 address.
	union
	{
		ULONG sin6_scope_id;     // Set of interfaces for a scope.
		SCOPE_ID sin6_scope_struct;
	};
} SOCKADDR_IN6_LH, *PSOCKADDR_IN6_LH, *LPSOCKADDR_IN6_LH;
typedef SOCKADDR_IN6_LH SOCKADDR_IN6;

int __stdcall WSAStartup(WORD, LPWSADATA);
int __stdcall WSACleanup(void);
int __stdcall WSAGetLastError(void);
SOCKET __stdcall socket(int, int, int);
u_long __stdcall htonl(u_long);
u_short __stdcall htons(u_short);
int __stdcall bind(SOCKET, const SOCKADDR*, int);
int __stdcall listen(SOCKET, int);
SOCKET __stdcall accept(SOCKET, const void *, int *);
int __stdcall closesocket(SOCKET);
int __stdcall connect(SOCKET, const void *, int);
int __stdcall recv(SOCKET, char *, int, int);
int __stdcall send(SOCKET, const char *, int, int);
int __stdcall ioctlsocket(SOCKET, long, u_long *);
int __stdcall getsockname(SOCKET, SOCKADDR *, int *);
INT __stdcall getaddrinfo(PCSTR, PCSTR, const ADDRINFOA *, PADDRINFOA *);
void __stdcall freeaddrinfo(PADDRINFOA);
PCSTR __stdcall inet_ntop(INT, const void *, PSTR, size_t);
int __stdcall shutdown(SOCKET, int);
int __stdcall select(int, fd_set *, fd_set *, fd_set *, const TIMEVAL *);
int __stdcall setsockopt(SOCKET, int, int, const void *, int);
int __stdcall getsockopt(SOCKET, int, int, void *, int *);

#ifdef __cplusplus
}
#endif

QWORD StringLength(const void *);

#endif //WSADEF_H
