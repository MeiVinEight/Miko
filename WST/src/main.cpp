#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <exception.h>

void func()
{
	String::string text = "https://github.com/MeiVinEight/Miko/commit/f960fe9cb3f803d5ee9aef414e21aa2dc9ec48e0";
	Crypto::SHA1 digest;
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