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
		Memory::string address = Memory::string(0);

		STRINGAPI
		string();
		STRINGAPI
		string(const char *);
		STRINGAPI
		string(const void *, QWORD);
		STRINGAPI
		string(const Memory::string &);
		STRINGAPI
		string(const Memory::string &, QWORD);
		STRINGAPI
		string(const String::string &);
		STRINGAPI
		string(String::string &&) noexcept;
		STRINGAPI
		~string();
		STRINGAPI
		String::string &operator=(const char *);
		STRINGAPI
		String::string &operator=(const String::string &) &;
		STRINGAPI
		String::string &operator=(String::string &&) & noexcept;
		STRINGAPI
		bool operator==(const char *) const;
		STRINGAPI
		bool operator==(const String::string &) const;
		STRINGAPI
		bool operator!=(const String::string &) const;
		STRINGAPI
		char &operator[](QWORD) const;
		STRINGAPI
		String::string &operator+=(const char *) &;
		STRINGAPI
		String::string &operator+=(const String::string &) &;
		STRINGAPI
		String::string operator+(const char *) const;
		STRINGAPI
		String::string operator+(const String::string &) const;
		STRINGAPI
		QWORD length() const;
	};

	STRINGAPI
	QWORD length(const void *);
	STRINGAPI
	String::string stringify(double);
	STRINGAPI
	String::string stringify(QWORD, bool);
	STRINGAPI
	String::string stringify(QWORD);
	STRINGAPI
	String::string stringify(int);
	STRINGAPI
	String::string stringify(void *);
	STRINGAPI
	double floating(const String::string &);
	STRINGAPI
	QWORD integer(const String::string &);
}
namespace Hexadecimal
{
	STRINGAPI String::string format(const Memory::string &);
}

#endif //MIKO_SSTRING_H
