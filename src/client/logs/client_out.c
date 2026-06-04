/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Log for the client
*/
#include "kronknet/client/client.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

[[gnu::hot]] void knClient_out(const knClient *client, const char *format, ...)
{
    va_list ap;

    if (!client || !client->shouldLog) {
        return;
    }
    char out[strlen(format) + strlen("[KRONKNET - OK] ") + 2];
    sprintf(out, "[KRONKNET - OK] %s\n", format);
    va_start(ap, format);
    vprintf(out, ap);
    va_end(ap);
}
