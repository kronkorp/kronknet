/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Logger impl (cloger from @clementd-tek)
*/
#include "clog.h"
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define RESET_COLOR "\033[0m"

const char *clog_level_names[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

const char *clog_level_colours[] = {
    "\x1b[94m",
    "\x1b[36m",
    "\x1b[32m",
    "\x1b[33m",
    "\x1b[31m",
    "\x1b[35m"
};

const char *clog_level_colours_disabled[] = {
    "",
    "",
    "",
    "",
    "",
    ""
};

clog_logger_t g_kronknet_logger = {
    .outs = {0},
    .level = CLOG_WARN,
    .colour_enabled = true,
    .timestamp_enabled = true,
};

static bool __file_is_a_tty(FILE *s) {
    if (s == NULL)
        return false;

    return isatty(fileno(s)) != 0;
}

static inline void timestamp(char buf[32], size_t size)
{
    time_t t = time(NULL);
    struct tm tmv;
    localtime_r(&t, &tmv);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", &tmv);
}

void clog_log(clog_logger_t *logger, clog_level_t level, const char *file, int line, const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    clog_vlog(logger, level, file, line, fmt, args);
    va_end(args);
}

void clog_vlog(clog_logger_t *logger, clog_level_t level, const char *file, int line, const char *fmt, va_list args) {
    char timebuf[32] = {0};
    const char * const * colours = logger->colour_enabled ? clog_level_colours : clog_level_colours_disabled;

    if (logger->level > level)
        return;
    timestamp(timebuf, 32);
    for (size_t i = 0; i < CLOG_NB_FILE; i++) {
        FILE *out = logger->outs[i];
        colours = __file_is_a_tty(out) && logger->colour_enabled ? clog_level_colours : clog_level_colours_disabled;
        if (out == NULL)
            continue;
        if (logger->timestamp_enabled) {
            fprintf(out, "%.*s ", 32, timebuf);
        }
        fprintf(out,
                "[%s%-5.5s%s] ",
                colours[level],
                clog_level_names[level],
                colours == clog_level_colours ? RESET_COLOR : "");
        if (file != NULL && line != 0)
            fprintf(out, "(%s:%d) ", file, line);
        va_list copy;
        va_copy(copy, args);
        vfprintf(out, fmt, copy);
        va_end(copy);
        fflush(out);
    }
}

void clog_set_log_level(clog_logger_t *logger, clog_level_t level) {
    if (logger == NULL)
        return;
    logger->level = level;
}

clog_level_t clog_get_log_level(clog_logger_t *logger) {
    if (logger == NULL)
        return CLOG_TRACE;
    return logger->level;
}

void clog_set_colour(clog_logger_t *logger, bool colour_enabled) {
    if (logger == NULL)
        return;
    logger->colour_enabled = colour_enabled;
}

void clog_set_timestamp_enabled(clog_logger_t *logger, bool timestamp_enabled) {
    if (logger == NULL)
        return;
    logger->timestamp_enabled = timestamp_enabled;
}

bool clog_add_out(clog_logger_t *logger, FILE *out)
{
    if (logger == NULL || out == NULL)
        return false;

    for (size_t i = 0; i < CLOG_NB_FILE; i++) {
        if (logger->outs[i] == out)
            return true;
        if (logger->outs[i] == NULL) {
            logger->outs[i] = out;
            return true;
        }
    }
    return false;
}

bool clog_remove_out(clog_logger_t *logger, FILE *out)
{
    if (logger == NULL || out == NULL)
        return false;

    for (size_t i = 0; i < CLOG_NB_FILE; i++) {
        if (logger->outs[i] == out) {
            logger->outs[i] = NULL;
            return true;
        }
    }

    return false;
}

void clog_logger_init(clog_logger_t *logger)
{
    if (logger == NULL)
        return;

    memset(logger, 0, sizeof(*logger));

    logger->level = CLOG_WARN;
    logger->colour_enabled = true;
    logger->timestamp_enabled = true;

    logger->outs[0] = stdout;
}
