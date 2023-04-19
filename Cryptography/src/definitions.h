#ifndef DEINITIONS_H
#define DEINITIONS_H

#define CRYPTO_SHARED
#include <crypto.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1

typedef unsigned char UCHAR, *PUCHAR;
typedef wchar_t WCHAR;
typedef const WCHAR *LPCWSTR;
typedef long NTSTATUS;
typedef DWORD ULONG;
typedef void *BCRYPT_ALG_HANDLE, *HINSTANCE;

extern BCRYPT_ALG_HANDLE RNG_HANDLE;

NTSTATUS __stdcall BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE, LPCWSTR, LPCWSTR, ULONG);
NTSTATUS __stdcall BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE, ULONG);
NTSTATUS __stdcall BCryptGenRandom(BCRYPT_ALG_HANDLE, PUCHAR, ULONG, ULONG);
QWORD GetAsLEndian(BYTE, const void *);
QWORD GetAsBEndian(BYTE, const void *);
void SaveAsLEndian(QWORD, BYTE, void *);
void SaveAsBEndian(QWORD, BYTE, void *);
bool Appendix32(QWORD, Memory::string &, QWORD &, void (*)(QWORD, BYTE, void *));
bool Appendix64(QWORD, Memory::string &, QWORD &, void (*)(QWORD, BYTE, void *));
void Transform32(Memory::string &, void (*)(QWORD, BYTE, void *));
void Transform64(Memory::string &, void (*)(QWORD, BYTE, void *));
QWORD ROTL32(QWORD, BYTE);
QWORD ROTR32(QWORD, BYTE);
QWORD ROTL64(QWORD, BYTE);
QWORD ROTR64(QWORD, BYTE);
QWORD Ch(QWORD, QWORD, QWORD);
QWORD Maj(QWORD, QWORD, QWORD);
QWORD SIGMA0256(QWORD);
QWORD SIGMA1256(QWORD);
QWORD Sigma0256(QWORD);
QWORD Sigma1256(QWORD);
QWORD SIGMA0512(QWORD);
QWORD SIGMA1512(QWORD);
QWORD Sigma0512(QWORD);
QWORD Sigma1512(QWORD);

#ifdef __cplusplus
}
#endif

#endif //DEINITIONS_H
