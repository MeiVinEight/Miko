#include <iostream>
#include <sstring.h>
#include <filesystem.h>

int main()
{
	String::string path = "C:/Windows";
	path = Filesystem::canonicalize(path);
	path = Filesystem::parent(path);
	return 0;
}