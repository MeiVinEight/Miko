#ifndef CRYPTO_SHA512_H
#define CRYPTO_SHA512_H

#include "WinType.h"

#ifdef __cplusplus
extern "C"
{
#endif


extern const QWORD SHA512K[80];

QWORD SIGMA0512(QWORD);
QWORD SIGMA1512(QWORD);
QWORD Sigma0512(QWORD);
QWORD Sigma1512(QWORD);
void CalculateSHA512(BYTE *, BYTE *);


#ifdef __cplusplus
}
#endif

#endif //CRYPTO_SHA512_H
