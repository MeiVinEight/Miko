#ifndef JSON_H
#define JSON_H

#ifdef JSON_SHARED
	#define JSONAPI __declspec(dllexport)
#else
	#define JSONAPI
#endif

#include <sstring.h>

typedef unsigned long long QWORD;

namespace JSON
{
	enum type
	{
		UNKNOWN,
		STRING,
		INTEGER,
		FLOAT,
		BOOLEAN,
		COMPONENT,
		ARRAY
	};

	class object
	{
		public:
		typedef void *(KV[2]);
		JSON::object::KV *content = nullptr;
		QWORD size = 0;
		BYTE type = JSON::type::UNKNOWN;

		object() = default;
		JSONAPI
		object(const JSON::object &);
		JSONAPI
		object(JSON::object &&) noexcept ;
		JSONAPI
		~object();
		JSONAPI
		JSON::object &operator=(const JSON::object &);
		JSONAPI
		JSON::object &operator=(JSON::object &&) noexcept;
		JSONAPI
		JSON::object &operator=(const char *);
		JSONAPI
		JSON::object &operator=(const String::string &);
		JSONAPI
		JSON::object &operator=(QWORD);
		JSONAPI
		JSON::object &operator=(int);
		JSONAPI
		JSON::object &operator=(double);
		JSONAPI
		JSON::object &operator=(bool);
		JSONAPI
		JSON::object &operator[](const String::string &);
		JSONAPI
		JSON::object &operator[](const char *);
		JSONAPI
		JSON::object &operator[](QWORD);
		JSONAPI
		operator String::string &() const;
		JSONAPI
		operator QWORD() const;
		JSONAPI
		operator int() const;
		JSONAPI
		operator double() const;
		JSONAPI
		operator bool() const;
		JSONAPI
		QWORD length() const;
		JSONAPI
		void cleanup();
		JSONAPI
		String::string stringify() const;
	};

	JSONAPI
	String::string stringify(const String::string &);
	JSONAPI
	String::string stringify(const JSON::object &);
	JSONAPI
	JSON::object resolve(const String::string &);
}

#endif //JSON_H
