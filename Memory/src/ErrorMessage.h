#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <memory.h>

extern DWORD (*RtlNtStatusToDosError)(long);
extern Memory::string *ErrorMessage;

#endif //ERRORMESSAGE_H
