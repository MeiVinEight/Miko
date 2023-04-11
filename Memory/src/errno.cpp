#include "definitions.h"

DWORD Memory::errno::registry(const char *msg)
{
	DWORD len = 0;
	while (msg[len++]);
	len--;
	Memory::string *newErrMsg = new Memory::string[ErrorCount + 1];
	for (DWORD i = 0; i < ErrorCount; i++)
	{
		newErrMsg[i] = (Memory::string &&) ErrorMessage[i];
	}
	delete[] ErrorMessage;
	ErrorMessage = newErrMsg;
	ErrorMessage[ErrorCount].resize(len);
	Memory::copy(ErrorMessage[ErrorCount].address, msg, len);
	return ErrorCount++;
}
Memory::string Memory::errno::message(DWORD code)
{
	if (code < ErrorCount)
	{
		return ErrorMessage[code];
	}
	return Memory::string(0);
}
