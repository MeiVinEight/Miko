#ifndef HEXADECIMAL_H
#define HEXADECIMAL_H

#include <stringspec.h>
#include <sstring.h>

namespace Hexadecimal
{
	STRINGAPI String::string format(const Memory::string &);
	STRINGAPI String::string stringify(QWORD);
}

#endif //HEXADECIMAL_H
