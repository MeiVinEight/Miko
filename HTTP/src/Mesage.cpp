#include "httpdef.h"

HTTP::Message::Message() = default;

HTTP::Message::Message(const HTTP::Message &copy):
	method(copy.method),
	URL(copy.URL),
	version(copy.version),
	status(copy.status),
	context(new String::string[copy.length][2]),
	length(copy.length),
	content(copy.content)
{
	for (QWORD i = 0; i < copy.length; i++)
	{
		this->context[i][0] = copy[i][0];
		this->context[i][1] = copy[i][1];
	}
}

HTTP::Message::Message(HTTP::Message &&move):
	method(move.method),
	URL((String::string &&)move.URL),
	version(move.version),
	status(move.status),
	context(move.context),
	length(move.length),
	content((Memory::string &&)move.content)
{
	move.method = 0;
	move.version = HTTP::HV_1_1;
	move.status = 0;
	move.context = nullptr;
	move.length = 0;
}

HTTP::Message::~Message()
{
	this->method = 0;
	this->version = HTTP::HV_1_1;
	this->status = 0;
	delete[] this->context;
	this->context = NULL;
	this->length = 0;
}

HTTP::Message &HTTP::Message::operator=(const HTTP::Message &copy)
{
	if (&copy != this)
	{
		(*this) = HTTP::Message(copy);
	}
	return *this;
}

HTTP::Message &HTTP::Message::operator=(HTTP::Message &&move)
{
	if (&move != this)
	{
		this->method = move.method;
		this->URL = (String::string &&)move.URL;
		this->version = move.version;
		this->status = move.status;
		delete[] this->context;
		this->context = move.context;
		this->length = move.length;
		this->content = (Memory::string &&)move.content;
		move.method = 0;
		move.version = HTTP::HV_1_1;
		move.status = 0;
		move.context = nullptr;
		move.length = 0;
	}
	return *this;
}

String::string &HTTP::Message::operator[](const String::string &key)
{
	if (!this->contain(key))
	{
		String::string (*ctx)[2] = new String::string[this->length + 1][2];
		for (QWORD i = 0; i < this->length; i++)
		{
			ctx[i][0] = this->context[i][0];
			ctx[i][1] = this->context[i][1];
		}
		ctx[this->length][0] = key;
		delete[] this->context;
		this->context = ctx;
		this->length++;
	}

	for (QWORD i = 0; i < this->length; i++)
	{
		if (this->context[i][0] == key)
		{
			return this->context[i][1];
		}
	}

	throw Exception::exception(((String::string)"Cannot found: " + key).address);
}

String::string *HTTP::Message::operator[](QWORD idx) const
{
	return this->context[idx];
}

bool HTTP::Message::contain(const String::string &key) const
{
	for (QWORD i = 0; i < this->length; i++)
	{
		if (this->context[i][0] == key)
		{
			return true;
		}
	}
	return false;
}