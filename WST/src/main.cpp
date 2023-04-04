#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <exception.h>

void func()
{
	String::string text = "74D388250CBE63C03D8A2EDA83D03D2FA0220CC98F0DA3072D973F2374D388250CBE63C";
	Cryptography::SHAKE md(Cryptography::SHAKE::SHAKE128, 127);
	md.update(text.address.address, text.length());
	Streaming::cout << Hexadecimal::format(md.finally());
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