/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Private logger impl
*/
#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

static const char *__type_names[] = {

    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"

};

void knLogger_log(
    knLoggerData *data,
    knLogLevel _type,
    const char *_file,
    int _line,
    const char * restrict _fmt,
    ...
)
{
    va_list args;

    if (!data || !data->out || _type < data->log_level)
        return;
    if (_file) {
        fprintf(data->out, "%s %s:%d: ", __type_names[_type], _file, _line);
    } else {
        fprintf(data->out, "%s: ", __type_names[_type]);
    }
    va_start(args, _fmt);
    vfprintf(data->out, _fmt, args);
    va_end(args);
    fprintf(data->out, "\n");
    fflush(data->out);
}
