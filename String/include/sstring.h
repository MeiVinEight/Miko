#ifndef MIKO_SSTRING_H
#define MIKO_SSTRING_H

#ifdef STRING_SHARED
	#define STRING_API __declspec(dllexport)
#else
	#define STRING_API
#endif

#include <memory.h>

namespace String
{
	STRING_API
	QWORD length(const void *);

	class string
	{
		public:
		Memory::string address = Memory::string(1);
		QWORD length = 0;

		STRING_API
		string();
		STRING_API
		string(const void *);
		STRING_API
		string(const void *, QWORD);
		STRING_API
		string(const String::string &);
		STRING_API
		string(String::string &&);
		STRING_API
		~string();
		STRING_API
		String::string &operator=(const void *);
		STRING_API
		String::string &operator=(const String::string &) &;
		STRING_API
		String::string &operator=(String::string &&) &;
		STRING_API
		bool operator==(const String::string &) const;
		STRING_API
		bool operator==(const void *) const;
		STRING_API
		char &operator[](QWORD) const;
		STRING_API
		String::string &operator+=(const String::string &) &;
		STRING_API
		String::string &operator+=(const void *) &;
		STRING_API
		String::string operator+(const String::string &) const;
		STRING_API
		String::string operator+(const void *) const;
	};
}

#endif //MIKO_SSTRING_H
