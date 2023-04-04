#ifndef KECCAK_H
#define KECCAK_H

#include "definitions.h"

void SPONGE(BYTE *, const BYTE *);
void PAD101(BYTE *, QWORD, QWORD &);

#endif //KECCAK_H
