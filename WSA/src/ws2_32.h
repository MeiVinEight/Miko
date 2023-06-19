#ifndef WS2_32_H
#define WS2_32_H

#ifdef __cplusplus
extern "C"
{
#endif


int __stdcall WSAGetLastError(void);
unsigned long __stdcall htonl(unsigned long);


#ifdef __cplusplus
}
#endif

#endif //WS2_32_H
