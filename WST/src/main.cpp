#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <exception.h>
#include <timestamp.h>

void func()
{
	Cryptography::MersenneTwister mt;
	mt.seed(Timestamp::current());
	for (int i = 0; i < 20; i++)
	{
		Streaming::cout << mt.random(10) << Streaming::LF;
	}
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