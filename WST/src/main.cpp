#include <sstring.h>
#include <streaming.h>
#include <crypto.h>
#include <filesystem.h>
#include <exception.h>
#include <timestamp.h>

void func()
{
	String::string base64 = "x3JJHMbDL1EzLkh9GBhXDw==";
	Memory::string data = Cryptography::BASE64::decode(base64.address);
	Streaming::cout << data.length;
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