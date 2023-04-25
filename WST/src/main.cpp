#include <sstring.h>
#include <streaming.h>
#include <filesystem.h>
#include <WTM.h>

DWORD ThreadProc(void *param)
{
	bool *flag = (bool *) param;
	while (*flag)
	{
		WTM::thread::sleep(1);
	}
	WTM::thread curr = WTM::thread::current();
	Streaming::cout << curr.name() << Streaming::LF;
	return 12138;
}

void func()
{
	bool flag = true;
	WTM::thread thread = WTM::thread::create(ThreadProc, &flag);
	thread.name((String::string("Sample text")).address);
	flag = false;
	thread.wait();
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