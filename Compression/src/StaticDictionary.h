#ifndef STATICDICTIONARY_H
#define STATICDICTIONARY_H
#include <memory.h>
#include "dictionary.h"

typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;

/* References: RFC7932 - Section 8.  Static Dictionary */
namespace StaticDictionary
{
	extern DWORD NWORDS[25];
	extern DWORD DOFFSET[25];
	extern DWORD NDBITS[25];

	/*
	 * Transformation:
	 * 0       Identity
	 * 1       FermentFirst
	 * 2       FermentAll
	 * 3..11   OmitFirst1..9
	 * 12..20  OmitLast1..9
	 */
	extern const char PREFIX_SUFFIX[];
	extern DWORD transformation[121][3];
	extern void (*(transforms[5]))(Memory::string &, WORD);


	extern DWORD ferment(Memory::string &, QWORD);
	void dictionary(Memory::string &, QWORD &, QWORD, QWORD, QWORD);
}

#endif //STATICDICTIONARY_H
