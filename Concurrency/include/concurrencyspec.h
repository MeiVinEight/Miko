#ifndef CONCURRENCYSPEC_H
#define CONCURRENCYSPEC_H

#ifdef CONCURRENCY_SHARED
#define CONCURRENCYAPI __declspec(dllexport)
#else
#define CONCURRENCYAPI __declspec(dllimport)
#endif

#endif //CONCURRENCYSPEC_H
