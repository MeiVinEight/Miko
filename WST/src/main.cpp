#include <sstring.h>
#include <streaming.h>
#include <crypto.h>

void func()
{
	Crypto::MD5 md5;
	String::string text = "https://github.com/MeiVinEight/Mikohttps://github.com/MeiVinEight/Miko";
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