#ifndef MIKO_SSTRING_H
#define MIKO_SSTRING_H

#ifdef STRING_SHARED
	#define STRINGAPI __declspec(dllexport)
#else
	#define STRINGAPI
#endif

#include <memory.h>

namespace String
{
	class string
	{
		public:
		Memory::string address = Memory::string(1);
		QWORD length = 0;

		STRINGAPI
		string();
		STRINGAPI
		string(const void *);
		STRINGAPI
		string(const void *, QWORD);
		STRINGAPI
		string(const String::string &);
		STRINGAPI
		string(String::string &&);
		STRINGAPI
		~string();
		STRINGAPI
		String::string &operator=(const void *);
		STRINGAPI
		String::string &operator=(const String::string &) &;
		STRINGAPI
		String::string &operator=(String::string &&) &;
		STRINGAPI
		bool operator==(const String::string &) const;
		STRINGAPI
		bool operator==(const void *) const;
		STRINGAPI
		char &operator[](QWORD) const;
		STRINGAPI
		String::string &operator+=(const String::string &) &;
		STRINGAPI
		String::string &operator+=(const void *) &;
		STRINGAPI
		String::string operator+(const String::string &) const;
		STRINGAPI
		String::string operator+(const void *) const;
	};

	STRINGAPI
	QWORD length(const void *);
	STRINGAPI
	String::string stringify(double);
	STRINGAPI
	double floating(const String::string &);
}

#endif //MIKO_SSTRING_H
