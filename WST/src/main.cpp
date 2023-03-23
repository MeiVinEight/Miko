#include <sstring.h>
#include <streaming.h>
#include <crypto.h>

void func()
{
	String::string text = "00000000000000000000000000000000000000000000000000000000";
	Crypto::SHA1 sha;
	sha.update(text.address.address, text.length());
	Streaming::cout << Hexadecimal::format(sha.value());
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