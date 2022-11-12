#ifndef WSA_WSADEF_H
#define WSA_WSADEF_H


#ifdef WSA_SHARED
	#define WSA_API __declspec(dllexport)
#else
	#define WSA_API
#endif

typedef int					BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;


#endif //WSA_WSADEF_H
