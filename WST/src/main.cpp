#include <sstring.h>
#include <streaming.h>
#include <crypto.h>

void func()
{
	Crypto::MD4 md4;
	String::string text = "https://github.com/MeiVinEight/Mikohttps://github.com/MeiVinEight/Miko";
	md4.update(text.address.address, text.length());
	Streaming::cout << Hexadecimal::format(md4.value());
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