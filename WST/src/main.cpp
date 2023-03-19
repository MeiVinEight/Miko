#include <streaming.h>
#include <crypto.h>

void func()
{
	Streaming::cout << (DWORD) Memory::BENDIAN << ' ' << (DWORD) Memory::LENDIAN;
}

int main()
{
	try
	{
		func();
	}
	catch (Exception::exception &exec)
	{
		Streaming::cout << exec.message;
	}
}