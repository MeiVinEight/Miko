#ifndef CWSSPEC_H
#define CWSSPEC_H

#ifdef CWS_SHARED
#define CWSAPI __declspec(dllexport)
#else
#define CWSAPI __declspec(dllimport)
#endif

#endif //CWSSPEC_H
