/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Log for the server
*/
#include "kronknet/server/server.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

[[gnu::hot]] void knServer_out(const knServer *server, const char *format, ...)
{
    va_list ap;

    if (!server || !server->logs) {
        return;
    }
    char out[strlen(format) + strlen("[KRONKNET - OK] ") + 2];
    sprintf(out, "[KRONKNET - OK] %s\n", format);
    va_start(ap, format);
    vprintf(out, ap);
    va_end(ap);
}
