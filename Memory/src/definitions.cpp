#include "definitions.h"
#pragma comment(lib, "DbgHelp.lib")

Memory::string *ErrorMessage = nullptr;
DWORD ErrorCode = 0;

void *__cdecl operator new(size_t size, void *where) noexcept
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
QWORD StringLength(const void *str)
{
	const char *s = (const char *) str;
	QWORD len = 0;
	while (s[len++]);
	return len - 1;
}