#ifndef HTTP_H
#define HTTP_H

#include <HTTPSec.h>
#include <WinType.h>
#include <memory.h>
#include <sstring.h>
#include <wsa.h>

namespace HTTP
{
	HTTPAPI extern const WORD RM_GET;
	HTTPAPI extern const WORD RM_HEAD;
	HTTPAPI extern const WORD RM_POST;
	HTTPAPI extern const WORD RM_PUT;
	HTTPAPI extern const WORD RM_DELETE;
	HTTPAPI extern const WORD RM_CONNECT;
	HTTPAPI extern const WORD RM_OPTIONS;
	HTTPAPI extern const WORD RM_TRACE;

	HTTPAPI extern const WORD HV_0_9;
	HTTPAPI extern const WORD HV_1_0;
	HTTPAPI extern const WORD HV_1_1;
	HTTPAPI extern const WORD HV_2_0;

	HTTPAPI extern const DWORD ERRNO_UNKNOWN_REQUEST_METHOD;
	HTTPAPI extern const DWORD ERRNO_UNKNOWN_STATUS_CODE;

	static const BYTE HTTP_CLIENT = 0;
	static const BYTE HTTP_SERVER = 1;

	HTTPAPI String::string method(WORD);
	HTTPAPI WORD method(const String::string &);
	HTTPAPI String::string status(WORD);
}

#endif //HTTP_H
