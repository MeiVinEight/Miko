#ifndef CRYPTO_SHA512_H
#define CRYPTO_SHA512_H

#include "definitions.h"

#ifdef __cplusplus
extern "C"
{
#endif


extern const QWORD SHA512K[80];

void CalculateSHA512(BYTE *, BYTE *);


#ifdef __cplusplus
}
#endif

#endif //CRYPTO_SHA512_H
