#ifndef COMPDEF_H
#define COMPDEF_H

#define COMPRESSION_SHARED
#include <exception.h>
#include <compression.h>

#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_ATTACH 1

typedef unsigned short WORD;
typedef unsigned long DWORD;

void DllMainStaticDictionary(unsigned int);


#endif //COMPDEF_H
