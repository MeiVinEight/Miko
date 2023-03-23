#include <sstring.h>
#include <streaming.h>
#include <crypto.h>

void func()
{
	String::string text = "123456789";
	Crypto::MD5 md5;
	md5.update(text.address.address, text.length());
	Streaming::cout << Hexadecimal::format(md5.value());
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