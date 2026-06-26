/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Private logger definition
*/
#ifndef KRONKNET_LOGGER_H
    #define KRONKNET_LOGGER_H
    #include <stdbool.h>
    #include <stdio.h>
    #include "kronknet/macros/types.h"

typedef struct kronknet_logger_data_s {

    FILE *out;
    bool log_level;

} knLoggerData;

void knLogger_log(knLoggerData *data, knLogLevel _type, const char *_file, int _line, const char * restrict _fmt, ...);

#ifndef KNDEBUG
    #define knTrace(data, fmt, ...) knLogger_log(&data, knLogTrace, NULL, 0, fmt, ##__VA_ARGS__)
    #define knDebug(data, fmt, ...) knLogger_log(&data, knLogDebug, NULL, 0, fmt, ##__VA_ARGS__)
    #define knInfo(data, fmt, ...) knLogger_log(&data, knLogInfo,  NULL, 0, fmt, ##__VA_ARGS__)
    #define knWarn(data, fmt, ...) knLogger_log(&data, knLogWarn,  NULL, 0, fmt, ##__VA_ARGS__)
    #define knError(data, fmt, ...) knLogger_log(&data, knLogError, NULL, 0, fmt, ##__VA_ARGS__)
    #define knFatal(data, fmt, ...) knLogger_log(&data, knLogFatal, NULL, 0, fmt, ##__VA_ARGS__)
#else
    #define knTrace(data, fmt, ...) knLogger_log(&data, knLogTrace, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define knDebug(data, fmt, ...) knLogger_log(&data, knLogDebug, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define knInfo(data, fmt, ...) knLogger_log(&data, knLogInfo,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define knWarn(data, fmt, ...) knLogger_log(&data, knLogWarn,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define knError(data, fmt, ...) knLogger_log(&data, knLogError, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define knFatal(data, fmt, ...) knLogger_log(&data, knLogFatal, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#endif /* KNDEBUG */

#endif /* KRONKNET_LOGGER_H */
