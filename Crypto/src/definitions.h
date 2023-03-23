#ifndef DEINITIONS_H
#define DEINITIONS_H

#define CRYPTO_SHARED
#include <crypto.h>

DWORD GetAsLEndian(const void *addr);
DWORD GetAsBEndian(const void *addr);
void SaveAsLEndian(DWORD, void *);
void SaveAsBEndian(DWORD, void *);
DWORD CircularLSH(DWORD, BYTE);

#endif //DEINITIONS_H
