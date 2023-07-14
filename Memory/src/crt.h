#ifndef CRT_H
#define CRT_H

#include <allocate.h>

#ifdef __cplusplus
extern "C"
{
#endif

void *memcpy(void *, const void *, unsigned long long);
void *memset(void *, int, unsigned long long);

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)

extern void *heap;

#ifdef __cplusplus
}
#endif

#endif //CRT_H
