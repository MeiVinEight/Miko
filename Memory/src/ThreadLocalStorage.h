#ifndef THREADLOCALSTORAGE_H
#define THREADLOCALSTORAGE_H

#include "CriticalSection.h"

#ifdef __cplusplus
extern "C"
{
#endif


extern CRITICAL_SECTION TLS_MUTEX;


int InitializeTLS();
int UninitializeTLS();


#ifdef __cplusplus
}
#endif

#endif //THREADLOCALSTORAGE_H
