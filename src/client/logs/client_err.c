/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Log for the server, on error for the client
*/
#include "kronknet/client/client.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

[[gnu::hot]] void knClient_err(const knClient *client, const char *format, ...)
{
    va_list ap;

    if (!client || !client->shouldLog) {
        return;
    }
    char err[strlen(format) + strlen("[KRONKNET - ERR] ") + 2];
    sprintf(err, "[KRONKNET - ERR] %s\n", format);
    va_start(ap, format);
    vdprintf(2, err, ap);
    va_end(ap);
}
