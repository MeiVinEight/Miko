#ifndef CRYPTO_SHA256_H
#define CRYPTO_SHA256_H

#include "definitions.h"

#ifdef __cplusplus
extern "C"
{
#endif


extern const DWORD SHA256K[64];

void CalculateSHA256(BYTE *, BYTE *);


#ifdef __cplusplus
}
#endif

#endif //CRYPTO_SHA256_H
