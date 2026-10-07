/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Portability macros for compiler-specific attributes.
*/

#ifndef KRONKNET_MACROS_OPTIMIZATION_H
    #define KRONKNET_MACROS_OPTIMIZATION_H

    // NOTE: __has_attribute(x) must not be reached when the compiler does not know it (MSVC):
    //       even behind a defined(__has_attribute) &&, it would not parse
    #if defined(__has_attribute)
        #define KN_HAS_ATTRIBUTE(x) __has_attribute(x)
    #else
        #define KN_HAS_ATTRIBUTE(x) 0
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4)
        #define KN_GNUC 1
    #else
        #define KN_GNUC 0
    #endif

    // NOTE: On Windows, the DLL exports its API (KRONKNET_BUILD_DLL, set when it is built),
    //       and who links with it can import it (KRONKNET_USE_DLL, set by the kronknet-shared target).
    //       Without either, nothing is needed: a static library needs none, and the API is only
    //       functions, which a DLL gives through its import library anyway
    #if defined(_WIN32) || defined(__CYGWIN__)
        #if defined(KRONKNET_BUILD_DLL)
            #define KN_API __declspec(dllexport)
        #elif defined(KRONKNET_USE_DLL)
            #define KN_API __declspec(dllimport)
        #else
            #define KN_API
        #endif
    #elif KN_GNUC || KN_HAS_ATTRIBUTE(visibility)
        #define KN_API __attribute__((visibility("default")))
    #else
        #define KN_API
    #endif

    #if KN_GNUC || KN_HAS_ATTRIBUTE(unused)
        #define KN_UNUSED __attribute__((unused))
    #else
        #define KN_UNUSED
    #endif

    #if KN_GNUC || KN_HAS_ATTRIBUTE(hot)
        #define KN_HOT __attribute__((hot))
    #else
        #define KN_HOT
    #endif

    #if KN_GNUC || KN_HAS_ATTRIBUTE(cold)
        #define KN_COLD __attribute__((cold))
    #else
        #define KN_COLD
    #endif

#endif /* KRONKNET_MACROS_OPTIMIZATION_H */
