#ifndef DEINITIONS_H
#define DEINITIONS_H

#define CRYPTO_SHARED
#include <crypto.h>

DWORD GetAsLittleEndian(const BYTE *);
DWORD CircularLSH(DWORD, BYTE);

#endif //DEINITIONS_H
