#include <streaming.h>
#include <crypto.h>

void func()
{
	String::string text = "https://github.com/MeiVinEight/Miko";
	Streaming::cout << text << Streaming::LF;
	Memory::string base64 = Crypto::BASE64::encode(text.address);
	Streaming::cout << String::string(base64) << Streaming::LF;
	Streaming::cout << String::string(Crypto::BASE64::decode(base64));
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