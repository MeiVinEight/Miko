#include <iostream>
#include <streaming.h>

void func()
{
	int x = 1;
	Streaming::cout << &x << Streaming::LF;
	std::cout << &x << std::endl;
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