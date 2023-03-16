#include "definitions.h"

String::string Exception::message(DWORD errcode)
{
	char buf[256]{0};
	DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD len = FormatMessageA(flags, nullptr, errcode, 0, buf, sizeof(buf), nullptr);
	if (buf[len - 2] == '\r' && buf[len - 1] == '\n')
	{
		len -= 2;
	}
	buf[len] = 0;
	return buf;
}