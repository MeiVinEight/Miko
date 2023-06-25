#include <exception.h>
#include <standardstream.h>
#include <hexadecimal.h>
#include <streaming.h>

void func()
{
}

int main()
{
	try
	{
		func();
	}
	catch (Memory::exception &exec)
	{
		Streaming::cout << exec.code << ": " << exec.message << Streaming::LF;
		for (DWORD i = 0; i < exec.count; i++)
		{
			Streaming::cout << exec.stack[i].offset << " (";
			if (exec.stack[i].function.length)
			{
				Streaming::cout << exec.stack[i].function << "+";
				Streaming::cout << Hexadecimal::stringify((QWORD) exec.stack[i].offset - (QWORD) exec.stack[i].address);
			}
			else
			{
				Streaming::cout << exec.stack[i].offset;
			}
			Streaming::cout << ") [";
			Streaming::cout << exec.stack[i].library << '+';
			Streaming::cout << Hexadecimal::stringify(((QWORD) exec.stack[i].offset) - ((QWORD) exec.stack[i].module));
			Streaming::cout << ']' << Streaming::LF;
		}
	}
}