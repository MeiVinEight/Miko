#ifndef GETASENDIAN_H
#define GETASENDIAN_H

#include <WinType.h>

#ifdef __cplusplus
extern "C"
{
#endif


QWORD GetAsLEndian(const void *, BYTE);
QWORD GetAsBEndian(const void *, BYTE);
void SaveAsLEndian(QWORD x, void *p, BYTE n);
void SaveAsBEndian(QWORD x, void *p, BYTE n);


#ifdef __cplusplus
}
#endif

#endif //GETASENDIAN_H
