#ifndef MEMDEF_H
#define MEMDEF_H

#define MEMORY_SHARED

#include "memory.h"

typedef unsigned __int64 size_t;

#ifdef __cplusplus
extern "C"
{
#endif
void* __cdecl malloc(size_t);
void* __cdecl realloc(void* , size_t);
void __cdecl free(void*);
void* __cdecl memcpy(void *, void const*, size_t);
void* __cdecl memset(void *, int, size_t);

#ifdef __cplusplus
}
#endif

#endif //MEMDEF_H
