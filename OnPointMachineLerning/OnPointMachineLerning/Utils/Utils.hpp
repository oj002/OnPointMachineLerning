#pragma once
#include <omp.h>

#include "Clock.hpp"
#include "Exeptions.hpp"
#include "Random.hpp"
#include "Tools.hpp"

#define OPML_MACRO_TO_STR(x) #x

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)

#if _OPML_ENABLE_OPENMP && defined(_OPENMP)
#define OPML_MACRO_APPEN_OMP(x) OPML_MACRO_TO_STR(omp x)
#define OPML_PRAGMA_OMP(x) _Pragma(OPML_MACRO_APPEN_OMP(#x)); // This needs to e done because _Pragma takes a parenthesized string literals
#else
#define OPML_PRAGMA_OMP(x)
#endif

#elif defined(_MSC_VER)

#if _OPML_ENABLE_OPENMP && defined(_OPENMP)
#define OPML_PRAGMA_OMP(x) __pragma(omp x)
#else
#define OPML_PRAGMA_OMP(x)
#endif

#endif
