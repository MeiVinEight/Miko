#ifndef COMPRESSIONSPEC_H
#define COMPRESSIONSPEC_H

#ifdef COMPRESSION_SHARED
#define COMPRESSIONAPI __declspec(dllexport)
#else
#define COMPRESSIONAPI __declspec(dllimport)
#endif

#endif //COMPRESSIONSPEC_H
