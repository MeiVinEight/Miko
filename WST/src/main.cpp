#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <wsa.h>
#include <cws.h>
#include <hhttp.h>

void func()
{
	Cryptography::SecurityRandom random;
	Streaming::cout << random.random() << Streaming::LF;
}

int main()
{
	try
	{
		func();
	}
	catch (Memory::exception &exec)
	{
		Streaming::cout << exec.message << Streaming::LF;
		for (DWORD i = 0; i < exec.count; i++)
		{
			/*
			Streaming::cout << exec.stack[i].address << ' ';
			Streaming::cout << exec.stack[i].offset << ' ';
			Streaming::cout << exec.stack[i].module << ' ';
			Streaming::cout << exec.stack[i].function << ' ';
			Streaming::cout << exec.stack[i].library << Streaming::LF;
			*/
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
			if (exec.stack[i].module)
			{
				Streaming::cout << exec.stack[i].library << '+';
				Streaming::cout << Hexadecimal::stringify(((QWORD) exec.stack[i].offset) - ((QWORD) exec.stack[i].module));
			}
			else
			{
				Streaming::cout << exec.stack[i].address;
			}
			Streaming::cout << ']' << Streaming::LF;
		}
	}
}