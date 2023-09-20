#ifndef FSSPEC_H
#define FSSPEC_H

#ifdef FS_SHARED
#define FSAPI __declspec(dllexport)
#else
#define FSAPI __declspec(dllimport)
#endif

#endif //FSSPEC_H
