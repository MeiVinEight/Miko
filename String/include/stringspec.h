#ifndef STRINGSPEC_H
#define STRINGSPEC_H

#ifdef STRING_SHARED
#define STRINGAPI __declspec(dllexport)
#else
#define STRINGAPI __declspec(dllimport)
#endif

#endif //STRINGSPEC_H
