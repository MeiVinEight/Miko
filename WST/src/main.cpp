#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <exception.h>

void func()
{
	String::string text = "D14A028C2A3A2BC9476102BB288234C415A2B01F828EA62AC5B3E42F";
	Crypto::SHA256 digest;
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