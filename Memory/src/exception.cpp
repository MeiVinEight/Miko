#include <memory.h>
#include <exception.h>
#include <optnew.h>

extern "C" WORD __stdcall RtlCaptureStackBackTrace(DWORD, DWORD, void *, void *);

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

Memory::exception::exception(DWORD code, BYTE type): code(code), type(type)
{
	this->message = Memory::message(code, type);
	backtrace(*this);
}
Memory::exception::exception(DWORD code): code(code), type(Memory::EXTERNAL)
{
	this->message = Memory::message(code, this->type);
	backtrace(*this);
}
Memory::exception::exception(const Memory::exception &exce) noexcept: code(exce.code), type(exce.type), message(exce.message)
{
	this->count = exce.count;
	this->stack = (Memory::exception::frame *) Memory::allocate(this->count * sizeof(Memory::exception::frame));
	for (DWORD i = 0; i < this->count; i++)
	{
		new (this->stack + i) Memory::exception::frame(exce.stack[i]);
	}
}
Memory::exception::~exception()
{
	for (DWORD i = 0; i < this->count; this->stack[i++].~frame());
	Memory::free(this->stack);
}