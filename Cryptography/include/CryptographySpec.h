#ifndef CRYPTOGRAPHYSPEC_H
#define CRYPTOGRAPHYSPEC_H

#ifdef CRYPTO_SHARED
#define CRYPTOAPI __declspec(dllexport)
#else
#define CRYPTOAPI __declspec(dllimport)
#endif

#endif //CRYPTOGRAPHYSPEC_H
