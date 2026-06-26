/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Portability macros for compiler-specific attributes.
*/

#ifndef KRONKNET_MACROS_OPTIMIZATION_H
    #define KRONKNET_MACROS_OPTIMIZATION_H

    // FIXME: Add Windows support when the platform is targeted.
    // #if defined(_WIN32) || defined(__CYGWIN__)
    //     #ifdef KRONKNET_BUILD_DLL
    //         #define KN_API __declspec(dllexport)
    //     #else
    //         #define KN_API __declspec(dllimport)
    //     #endif
    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(visibility)
        #define KN_API __attribute__((visibility("default")))
    #else
        #define KN_API
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(unused)
        #define KN_UNUSED __attribute__((unused))
    #else
        #define KN_UNUSED
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(hot)
        #define KN_HOT __attribute__((hot))
    #else
        #define KN_HOT
    #endif

    #if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__has_attribute) && __has_attribute(cold)
        #define KN_COLD __attribute__((cold))
    #else
        #define KN_COLD
    #endif

#endif /* KRONKNET_MACROS_OPTIMIZATION_H */
