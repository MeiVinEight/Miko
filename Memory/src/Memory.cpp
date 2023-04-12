#include "definitions.h"

const BYTE Memory::INTERNAL = 0;
const BYTE Memory::EXTERNAL = 1;

DWORD order_value = 1;
char *order_point = (char *) &order_value;
const BYTE Memory::BENDIAN = order_point[0] == 0;
const BYTE Memory::LENDIAN = order_point[0] == 1;
const DWORD Memory::ERRNO_SUCCESS = Memory::registry("The operation completed successfully");
const DWORD Memory::ERRNO_ACCESS_VIOLATION = Memory::registry("Memory access violation");
const DWORD Memory::ERRNO_INVALID_PARAMETER = Memory::registry("The parameter is incorrect");

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
		case Memory::INTERNAL:
		{
			char buf[256]{0};
			DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			DWORD len = FormatMessageA(flags, nullptr, errcode, 0, buf, sizeof(buf), nullptr);
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
			throw Memory::exception(Memory::ERRNO_INVALID_PARAMETER);
		}
	}
	return msg;
}