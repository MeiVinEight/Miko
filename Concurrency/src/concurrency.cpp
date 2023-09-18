#include <exception.h>
#include <concurrency.h>

const DWORD Concurrency::ERRNO_LOCK_BELONG = Memory::registry("The lock dose not belong current thread");