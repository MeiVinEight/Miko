#ifndef DEINITIONS_H
#define DEINITIONS_H

#define CRYPTO_SHARED
#include <crypto.h>

QWORD GetAsLEndian(BYTE, const void *addr);
QWORD GetAsBEndian(BYTE, const void *addr);
void SaveAsLEndian(QWORD, BYTE, void *);
void SaveAsBEndian(QWORD, BYTE, void *);
QWORD ROTL(QWORD, BYTE);
QWORD ROTR(QWORD, BYTE);

#endif //DEINITIONS_H
