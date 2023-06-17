#ifndef MIKO_TIMESTAMPSPEC_H
#define MIKO_TIMESTAMPSPEC_H

#ifdef TIMESTAMP_SHARED
#define TIMEAPI __declspec(dllexport)
#else
#define TIMEAPI __declspec(dllimport)
#endif

#endif //MIKO_TIMESTAMPSPEC_H
