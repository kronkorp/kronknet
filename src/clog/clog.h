/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Logger header (cloger from @clementd-tek)
*/
#ifndef CLOG_H
    #define CLOG_H
    #include <stdbool.h>
    #include <stdarg.h>
    #include <stdio.h>
    #define CLOG_NB_FILE 8

typedef enum {

    CLOG_TRACE = 0,
    CLOG_DEBUG,
    CLOG_INFO,
    CLOG_WARN,
    CLOG_ERROR,
    CLOG_FATAL,
    CLOG_SIZE

} clog_level_t;

typedef struct {

    FILE *outs[CLOG_NB_FILE];
    clog_level_t level;
    bool colour_enabled;
    bool timestamp_enabled;

} clog_logger_t;

extern clog_logger_t g_kronknet_logger;

void clog_log(clog_logger_t *logger, clog_level_t level, const char *file, int line, const char *fmt, ...);
void clog_vlog(clog_logger_t *logger, clog_level_t level, const char *file, int line, const char *fmt, va_list args);
void clog_set_log_level(clog_logger_t *logger, clog_level_t level);
clog_level_t clog_get_log_level(clog_logger_t *logger);
void clog_set_colour(clog_logger_t *logger, bool colour_enabled);
void clog_set_timestamp_enabled(clog_logger_t *logger, bool timestamp_enabled);

bool clog_add_out(clog_logger_t *logger, FILE *out);
bool clog_remove_out(clog_logger_t *logger, FILE *out);
void clog_logger_init(clog_logger_t *logger);

#ifdef DEBUG
    #define clog_trace(...) clog_log(&g_kronknet_logger, CLOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
    #define clog_debug(...) clog_log(&g_kronknet_logger, CLOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
    #define clog_info(...)  clog_log(&g_kronknet_logger, CLOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
    #define clog_warn(...)  clog_log(&g_kronknet_logger, CLOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
    #define clog_error(...) clog_log(&g_kronknet_logger, CLOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
    #define clog_fatal(...) clog_log(&g_kronknet_logger, CLOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define clog_trace(...) clog_log(&g_kronknet_logger, CLOG_TRACE, NULL, 0, __VA_ARGS__)
    #define clog_debug(...) clog_log(&g_kronknet_logger, CLOG_DEBUG, NULL, 0, __VA_ARGS__)
    #define clog_info(...)  clog_log(&g_kronknet_logger, CLOG_INFO,  NULL, 0, __VA_ARGS__)
    #define clog_warn(...)  clog_log(&g_kronknet_logger, CLOG_WARN,  NULL, 0, __VA_ARGS__)
    #define clog_error(...) clog_log(&g_kronknet_logger, CLOG_ERROR, NULL, 0, __VA_ARGS__)
    #define clog_fatal(...) clog_log(&g_kronknet_logger, CLOG_FATAL, NULL, 0, __VA_ARGS__)
#endif /* DEBUG */

#endif /* CLOG_H */
