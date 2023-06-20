#include "definitions.h"

void *NTDLL = 0;
long (*ZwSuspendProcess)(void *) = 0;
long (*ZwResumeProcess)(void *) = 0;
long (__stdcall *NtQueryInformationProcess)(void *, DWORD, void *, DWORD, DWORD *) = 0;