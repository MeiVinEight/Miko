#ifndef CRYPTOGRAPHY_SHA3_H
#define CRYPTOGRAPHY_SHA3_H

#include "Keccak.h"

void KeccakP(BYTE, BYTE, BYTE *, BYTE *);
void KeccakF(BYTE, BYTE *, BYTE *);
void Sponge(BYTE *, const BYTE *);
void PAD101(BYTE *, QWORD, QWORD &);

#endif //CRYPTOGRAPHY_SHA3_H
