#ifndef CPP11_DEFINES_H
#define CPP11_DEFINES_H

// gcc supports constexpr since 4.6
#ifdef __GNUG__
    #if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 6)
        #define CONSTEXPR constexpr
    #else
        #define CONSTEXPR
    #endif
#endif

// clang supports constexpr since 3.1
#ifdef __clang__
    #if (__clang_major__ >= 3 && __clang_minor__ >= 1)
        #define CONSTEXPR constexpr
    #else
        #define CONSTEXPR
    #endif
#endif

// visual c++ (2013rc) still no support for constexpr
#ifdef _MSC_VER
    #define CONSTEXPR
#endif

#ifndef CONSTEXPR
    #define CONSTEXPR
#endif

#endif  // CPP11_DEFINES_H