#include <sstring.h>
#include <filesystem.h>
#include <streaming.h>

int main()
{
	Streaming::file file = Streaming::file("sample.txt");
	file.write("Hello World", 11);
	file.flush();
	file.close();
}