#ifndef CMAKE_BUILD

#include "CriticalSection.h"
#include "ConditionVariable.h"
#include "ThreadLocalStorage.h"

CRITICAL_SECTION TLS_MUTEX;
CONDITION_VARIABLE TLS_CONVAR;
__declspec(dllexport) unsigned long _tls_index = 0;

int InitializeTLS()
{
	InitializeCriticalSection(&TLS_MUTEX);
	InitializeConditionVariable(&TLS_CONVAR);
	return 1;
}
int UninitializeTLS()
{
	DeleteCriticalSection(&TLS_MUTEX);
	return 1;
}


__declspec(dllexport) void _Init_thread_header(int *pOnce)
{
	EnterCriticalSection(&TLS_MUTEX);
	while (1)
	{
		if (*pOnce != -1)
		{
			if (!*pOnce)
				*pOnce = -1;
			break;
		}
		SleepConditionVariableCS(&TLS_CONVAR, &TLS_MUTEX, -1);
	}
	LeaveCriticalSection(&TLS_MUTEX);
}
__declspec(dllexport) void _Init_thread_footer(int *pOnce)
{
	EnterCriticalSection(&TLS_MUTEX);
	// mov rax, gs:58h
	// mov rax, [rax+rdx*8]  ;rdx = 0
	// mov rax, [rax+r9]     ;r9  = 104h
	*pOnce = (int) 0x80000000;
	LeaveCriticalSection(&TLS_MUTEX);
	WakeAllConditionVariable(&TLS_CONVAR);
}
__declspec(dllexport) void _Init_thread_abort(int *pOnce)
{
	EnterCriticalSection(&TLS_MUTEX);
	*pOnce = 0;
	LeaveCriticalSection(&TLS_MUTEX);
	WakeAllConditionVariable(&TLS_CONVAR);
}

#endif