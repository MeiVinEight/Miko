#ifndef WTMSPEC_H
#define WTMSPEC_H


#ifdef WTM_SHARED
#define WTMAPI __declspec(dllexport)
#else
#define WTMAPI __declspec(dllimport)
#endif

#endif //WTMSPEC_H
