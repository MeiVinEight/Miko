#ifndef WSASPEC_H
#define WSASPEC_H

#undef WSAAPI
#ifdef WSA_SHARED
#define WSAAPI __declspec(dllexport)
#else
#define WSAAPI __declspec(dllimport)
#endif

#endif //WSASPEC_H
