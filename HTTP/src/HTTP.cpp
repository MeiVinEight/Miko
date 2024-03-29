#include <exception.h>
#include <hhttp.h>

const QWORD RM_TYPE_COUNT = 9;

const WORD HTTP::RM_GET        = 1;
const WORD HTTP::RM_HEAD       = 2;
const WORD HTTP::RM_POST       = 3;
const WORD HTTP::RM_PUT        = 4;
const WORD HTTP::RM_DELETE     = 5;
const WORD HTTP::RM_CONNECT    = 6;
const WORD HTTP::RM_OPTIONS    = 7;
const WORD HTTP::RM_TRACE      = 8;
const WORD HTTP::HV_0_9        = ((0 << 8) | 9);
const WORD HTTP::HV_1_0        = ((1 << 8) | 0);
const WORD HTTP::HV_1_1        = ((1 << 8) | 1);
const WORD HTTP::HV_2_0        = ((2 << 8) | 0);

const DWORD HTTP::ERRNO_UNKNOWN_REQUEST_METHOD = Memory::registry("Unknown request method");
const DWORD HTTP::ERRNO_UNKNOWN_STATUS_CODE = Memory::registry("Unknown status code");

String::string METHODS[RM_TYPE_COUNT] = {
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"CONNECT",
	"OPTIONS",
	"TRACE",
};

String::string HTTP::method(WORD id)
{
	id--;
	if (id < RM_TYPE_COUNT)
	{
		return METHODS[id];
	}
	throw Memory::exception(HTTP::ERRNO_UNKNOWN_REQUEST_METHOD);
}
WORD HTTP::method(const String::string &str)
{
	for (WORD id = 0; id < RM_TYPE_COUNT; id++)
		if (METHODS[id] == str)
			return id + 1;
	throw Memory::exception(HTTP::ERRNO_UNKNOWN_REQUEST_METHOD);
}
String::string HTTP::status(WORD stat)
{
	switch (stat)
	{
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Unused";
		case 307: return "Temporary Redirect";
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timed-out";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Request Entity Too Large";
		case 414: return "Request-URL Too Large";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested range not satisfiable";
		case 417: return "Expectation Failed";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timed-out";
		case 505: return "HTTP Version not supported";
	}
	throw Memory::exception(HTTP::ERRNO_UNKNOWN_STATUS_CODE);
}