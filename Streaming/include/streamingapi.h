#ifndef STREAMINGAPI_H
#define STREAMINGAPI_H

#ifdef STREAMING_SHARED
#define STREAMINGAPI __declspec(dllexport)
#else
#define STREAMINGAPI __declspec(dllimport)
#endif

#endif //STREAMINGAPI_H
