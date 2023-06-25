#include <memory.h>
#include <exception.h>
#include <optnew.h>
#include <winerr.h>

#include "ErrorMessage.h"

#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
#define FORMAT_MESSAGE_FROM_STRING     0x00000400
#define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF

extern "C"
{

DWORD FormatMessageA(DWORD, const void *, DWORD, DWORD, char *, DWORD, char *);
DWORD GetLastError(void);
void SetLastError(DWORD);
WORD __stdcall RtlCaptureStackBackTrace(DWORD, DWORD, void *, void *);

}

DWORD (*RtlNtStatusToDosError)(long) = nullptr;
Memory::string *ErrorMessage = nullptr;
DWORD ErrorCode = 0;

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
DWORD Memory::registry(const char *msg)
{
	DWORD len = 0;
	while (msg[len++]);
	len--;
	Memory::string *newErrMsg = new Memory::string[ErrorCode + 1];
	for (DWORD i = 0; i < ErrorCode; i++)
	{
		newErrMsg[i] = (Memory::string &&) ErrorMessage[i];
	}
	delete[] ErrorMessage;
	ErrorMessage = newErrMsg;
	ErrorMessage[ErrorCode].resize(len);
	Memory::copy(ErrorMessage[ErrorCode].address, msg, len);
	return ErrorCode++;
}
Memory::string Memory::message(DWORD errcode, BYTE type)
{
	Memory::string msg;
	switch (type)
	{
		case Memory::NTSTATUS:
		case Memory::DOSERROR:
		{
			if (type == Memory::NTSTATUS)
				errcode = RtlNtStatusToDosError((long) errcode);
			char buf[256]{0};
			DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			// EN US = 0x0409
			DWORD len = FormatMessageA(flags, nullptr, errcode, 0x0409, buf, sizeof(buf), nullptr);
			if (len == 0)
				throw Memory::exception(Memory::error(), Memory::DOSERROR);
			// trialing blanks
			while (len && (buf[--len] <= 0x20));
			len++;
			msg.resize(len);
			Memory::copy(msg.address, buf, len);
			break;
		}
		case Memory::EXTERNAL:
		{
			if (errcode < ErrorCode)
			{
				msg = ErrorMessage[errcode];
				break;
			}
		}
		default:
		{
			throw Memory::exception(ERROR_INVALID_PARAMETER, Memory::DOSERROR);
		}
	}
	return msg;
}
DWORD Memory::error()
{
	return GetLastError();
}
void Memory::error(DWORD code)
{
	SetLastError(code);
}