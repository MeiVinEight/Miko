#ifndef ROTATE_H
#define ROTATE_H

#include <WinType.h>

#ifdef __cplusplus
extern "C"
{
#endif

QWORD ROTL(QWORD, BYTE, BYTE);
QWORD ROTR(QWORD, BYTE, BYTE);
QWORD ROTL32(QWORD, BYTE);
QWORD ROTR32(QWORD, BYTE);
QWORD ROTL64(QWORD, BYTE);
QWORD ROTR64(QWORD, BYTE);

#ifdef __cplusplus
}
#endif

#endif //ROTATE_H
