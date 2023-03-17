#include <streaming.h>
#include <crypto.h>

void func()
{
	String::string text = "https://github.com/MeiVinEight/Miko";
	Crypto::CRC crc(Crypto::CRC::CRC64_ECMA);
	crc.update(text.address.address, text.length());
	Streaming::cout << crc.value();
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