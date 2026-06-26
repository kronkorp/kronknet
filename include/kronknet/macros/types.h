/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** macros/types.h for explicit types definitions
*/
#ifndef KRONKNET_MACROS_TYPES_H
    #define KRONKNET_MACROS_TYPES_H
    #include <stdint.h>

    #define KNBUFFSIZ 8192

    // NOTE: Bool definition
    typedef int knBool;
    #define knTrue  1
    #define knFalse 0

    // NOTE: port type definition
    typedef unsigned short int knPort;

    // NOTE: Loging levels
    typedef enum kronknet_log_level_e {

        knLogTrace = 0,
        knLogDebug,
        knLogInfo,
        knLogWarn,
        knLogError,
        knLogFatal,
        // NOTE: Just log nothing...
        knLogNone,

    } knLogLevel;


#endif /* KRONKNET_MACROS_TYPES_H */
