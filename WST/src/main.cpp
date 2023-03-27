#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <exception.h>

void func()
{
	String::string text = "00000000000000000000000000000000000000000000000000000000";
	Crypto::MD5 digest;
	digest.update(text.address.address, text.length());
	Streaming::cout << Hexadecimal::format(digest.value());
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