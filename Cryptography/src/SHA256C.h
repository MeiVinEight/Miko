#ifndef CRYPTO_SHA256_H
#define CRYPTO_SHA256_H

#include <WinType.h>

#ifdef __cplusplus
extern "C"
{
#endif


extern const DWORD SHA256K[64];

QWORD SIGMA0256(QWORD);
QWORD SIGMA1256(QWORD);
QWORD Sigma0256(QWORD);
QWORD Sigma1256(QWORD);
void CalculateSHA256(BYTE *, BYTE *);


#ifdef __cplusplus
}
#endif

#endif //CRYPTO_SHA256_H
