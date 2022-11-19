#include "httpdef.h"

String::string METHODS[] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"CONNECT",
	"OPTIONS",
	"TRACE",
	"PATCH"
};

String::string HTTP::method(WORD id)
{
	id--;
	if (id < 9)
	{
		return METHODS[id];
	}
	throw Exception::exception("Unknown request method");
}