#ifndef DEINITIONS_H
#define DEINITIONS_H

#include <WinType.h>
#include <memory.h>

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
extern BYTE KECCAKRC[255];

NTSTATUS __stdcall BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE, LPCWSTR, LPCWSTR, DWORD);
NTSTATUS __stdcall BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE, DWORD);
NTSTATUS __stdcall BCryptGenRandom(BCRYPT_ALG_HANDLE, PUCHAR, DWORD, DWORD);
QWORD GetAsLEndian(BYTE, const void *);
QWORD GetAsBEndian(BYTE, const void *);
void SaveAsLEndian(QWORD, BYTE, void *);
void SaveAsBEndian(QWORD, BYTE, void *);
bool Appendix32(QWORD, Memory::string &, QWORD &, void (*)(QWORD, void *, BYTE));
bool Appendix64(QWORD, Memory::string &, QWORD &, void (*)(QWORD, void *, BYTE));
void Transform32(Memory::string &, void (*)(QWORD, void *, BYTE));
void Transform64(Memory::string &, void (*)(QWORD, void *, BYTE));
QWORD ROTL(QWORD, BYTE, BYTE);
QWORD ROTR(QWORD, BYTE, BYTE);
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
