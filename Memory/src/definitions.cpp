#include "definitions.h"
#pragma comment(lib, "DbgHelp.lib")

Memory::string *ErrorMessage = nullptr;
DWORD ErrorCode = 0;

inline __cdecl void *operator new(size_t size, void *where) noexcept
{
	(void) size;
	return where;
}
void backtrace(Memory::exception &exec)
{
	DWORD max = 64 * sizeof(void *);
	DWORD count = 0;
	void **arr = (void **) Memory::allocate(max);
	void *retAddr;
	while (RtlCaptureStackBackTrace(count + 2, 1, &retAddr, nullptr))
	{
		if (count >= max / sizeof(void *))
		{
			max *= 2;
			arr = (void **) Memory::reallocate(arr, max);
		}
		arr[count++] = retAddr;
	}

	exec.stack = (Memory::exception::frame *) Memory::allocate(count * sizeof(Memory::exception::frame));
	for (DWORD i = 0; i < count; new (exec.stack + i) Memory::exception::frame(arr[i]), i++);
	exec.count = count;

	Memory::free(arr);
}
QWORD strlen(const void *str)
{
	const char *s = (const char *) str;
	while (*s++);
	return s - ((const char *)str) - 1;
}