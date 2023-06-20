#ifndef HTTPSEC_H
#define HTTPSEC_H

#ifdef HTTP_SHARED
#define HTTPAPI __declspec(dllexport)
#else
#define HTTPAPI __declspec(dllimport)
#endif

#endif //HTTPSEC_H
