/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Log for the server, on error
*/
#include "kronknet/server/server.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

[[gnu::hot]] void knServer_err(const knServer *server, const char *format, ...)
{
    va_list ap;

    if (!server || !server->logs) {
        return;
    }
    char err[strlen(format) + strlen("[KRONKNET - ERR] ") + 2];
    sprintf(err, "[KRONKNET - ERR] %s\n", format);
    va_start(ap, format);
    vdprintf(2, err, ap);
    va_end(ap);
}
