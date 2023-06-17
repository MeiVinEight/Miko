#ifndef CRITICALSECTION_H
#define CRITICALSECTION_H

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct __declspec(align(8)) RTL_CRITICAL_SECTION
{
	void *DebugInfo;
	long LockCount;
	long RecursionCount;
	void *OwningThread;
	void *LockSemaphore;
	unsigned long long SpinCount;
} CRITICAL_SECTION, *PCRITICAL_SECTION;

void __stdcall InitializeCriticalSection(PCRITICAL_SECTION);
void __stdcall EnterCriticalSection(PCRITICAL_SECTION);
void __stdcall LeaveCriticalSection(PCRITICAL_SECTION);
void __stdcall DeleteCriticalSection(PCRITICAL_SECTION);


#ifdef __cplusplus
}
#endif

#endif //CRITICALSECTION_H
