#include "definitions.h"

String::string Hexadecimal::format(const Memory::string &data)
{
	static char hex[] = "0123456789ABCDEF";
	Memory::string output(data.length << 1);
	for (QWORD i = 0; i < data.length; i++)
	{
		output[i * 2 + 0] = hex[(data[i] >> 4) & 0xF];
		output[i * 2 + 1] = hex[(data[i] >> 0) & 0xF];
	}
	return output;
}