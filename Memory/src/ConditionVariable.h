#ifndef CONDITIONVARIABLE_H
#define CONDITIONVARIABLE_H

#include "CriticalSection.h"

typedef struct __declspec(align(8)) RTL_CONDITION_VARIABLE
{
	void *Ptr;
} CONDITION_VARIABLE, *PCONDITION_VARIABLE;

void __stdcall InitializeConditionVariable(PCONDITION_VARIABLE);
int __stdcall SleepConditionVariableCS(PCONDITION_VARIABLE, PCRITICAL_SECTION, unsigned long);
void __stdcall WakeAllConditionVariable(PCONDITION_VARIABLE);

#endif //CONDITIONVARIABLE_H
