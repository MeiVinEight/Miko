#ifndef CRT_H
#define CRT_H

#ifdef __cplusplus
extern "C"
{
#endif


void *__cdecl malloc(unsigned long long);
void *__cdecl realloc(void *, unsigned long long);
void __cdecl free(void *);
void *memcpy(void *, const void *, unsigned long long);
void *memset(void *, int, unsigned long long);

#pragma intrinsic(memcpy)
#pragma intrinsic(memset)

extern void *heap;

#ifdef __cplusplus
}
#endif

#endif //CRT_H
