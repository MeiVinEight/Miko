#include <sstring.h>
#include <streaming.h>
#include <filesystem.h>
#include <wsa.h>
#include <hhttp.h>
#include <cws.h>

void func()
{
	String::string text = "sample text";
	Cryptography::SHA3 sha3(Cryptography::SHA3::SHA256);
	sha3.update(text.address.address, text.length());
	Streaming::cout << Hexadecimal::format(sha3.finally());
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