#ifndef CRYPTO_SHA256_H
#define CRYPTO_SHA256_H

#include "definitions.h"

extern DWORD SHA256K[64];

void CalculateSHA256(BYTE *, BYTE *);

#endif //CRYPTO_SHA256_H
