#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <memory.h>

extern ULONG (*RtlNtStatusToDosError)(NTSTATUS);
extern Memory::string *ErrorMessage;

#endif //ERRORMESSAGE_H
