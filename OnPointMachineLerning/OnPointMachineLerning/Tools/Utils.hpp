#pragma once
#include <omp.h>

#define OPML_MACRO_TO_STR(x) #x

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)

#if _OPML_ENABLE_OPENMP && defined(_OPENMP)
#define OPML_MACRO_APPEN_OMP(x) OPML_MACRO_TO_STR(omp x)
#define OPML_PRAGMA_OMP(x) _Pragma(OPML_MACRO_APPEN_OMP(#x)); // This needs to e done because _Pragma takes a parenthesized string literals
#else
#define OPML_PRAGMA_OMP(x)
#endif

#elif defined(_MSC_VER)
#define OPML_PRAGMA_OMP(x) __pragma(omp x)

#if _OPML_ENABLE_OPENMP && defined(_OPENMP)
#define OPML_PRAGMA_OMP(x) __pragma(omp x)
#else
#define OPML_PRAGMA_OMP(x)
#endif

#endif

/*
#if _OPML_ENABLE_OPENMP && defined(_OPENMP)
#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#define OPML_MACRO_TO_STR(x) #x
#define OPML_MACRO_APPEN_OMP(x) OPML_MACRO_TO_STR(omp x)
#define OPML_PRAGMA_OMP(x) _Pragma(OPML_MACRO_APPEN_OMP(#x)); // This needs to e done because _Pragma takes a parenthesized string literals
#elif defined(_MSC_VER)
#define OPML_PRAGMA_OMP(x) __pragma(omp x)
#endif
#else // _OPML_ENABLE_OPENMP
#define OPML_PRAGMA_OMP(x)
#endif // _OPML_ENABLE_OPENMP
*/