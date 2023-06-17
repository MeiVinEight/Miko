#include "definitions.h"
#include "crt.h"
#include "ErrorMessage.h"

#include <error.h>
#include <exception.h>

#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
#define FORMAT_MESSAGE_FROM_STRING     0x00000400
#define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF

extern "C"
{

DWORD FormatMessageA(DWORD, const void *, DWORD, DWORD, LPSTR, DWORD, char *);
DWORD GetLastError(void);

}

ULONG (*RtlNtStatusToDosError)(NTSTATUS) = nullptr;
Memory::string *ErrorMessage = nullptr;

DWORD ErrorCode = 0;
DWORD order_value = 1;
char *order_point = (char *) &order_value;

const BYTE Memory::BENDIAN = order_point[0] == 0;
const BYTE Memory::LENDIAN = order_point[0] == 1;
const DWORD Memory::ERRNO_SUCCESS = Memory::registry("The operation completed successfully");
const DWORD Memory::ERRNO_ACCESS_VIOLATION = Memory::registry("Memory access violation");
const DWORD Memory::ERRNO_OBJECT_CLOSED = Memory::registry("Kernel object has been closed");

void *Memory::allocate(QWORD size)
{
	return malloc(size);
}
void *Memory::reallocate(void *p, QWORD size)
{
	return realloc(p, size);
}
void Memory::free(void *p)
{
	::free(p);
}
void Memory::copy(void *dst, const void *src, QWORD len)
{
	memcpy(dst, src, len);
}
void Memory::fill(void *p, BYTE x, QWORD size)
{
	memset(p, x, size);
}
bool Memory::compare(const void *p1, const void *p2, QWORD length)
{
	const char *s1 = (const char *)p1;
	const char *s2 = (const char *)p2;
	bool ret = true;
	for (QWORD i = 0; ret && i < length; i++)
	{
		ret &= s1[i] == s2[i];
	}
	return ret;
}
void Memory::reverse(void *b, QWORD len)
{
	BYTE *buf = (BYTE *) b;
	for (QWORD i = 0; i < len >> 1; i++)
	{
		BYTE t = buf[i];
		buf[i] = buf[len - i - 1];
		buf[len - i - 1] = t;
	}
}
void Memory::violation()
{
	*((BYTE *)1) = 0; // Access violation
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
				errcode = RtlNtStatusToDosError((LONG) errcode);
			char buf[256]{0};
			DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			// EN US = 0x0409
			DWORD len = FormatMessageA(flags, nullptr, errcode, 0x0409, buf, sizeof(buf), nullptr);
			if (len == 0)
				throw Memory::exception(GetLastError(), Memory::DOSERROR);
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
