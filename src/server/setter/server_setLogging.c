/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set the logging, on or off.
*/
#include "kronknet/callback/callback.h"
#include "kronknet/macros/types.h"
#include <stdarg.h>
#include <stdbool.h>
#include "../server.h"

void knServer_setLogLevel(
    knServer *server,
    knLogLevel level
)
{
    if (!server || level < knLogTrace || level > knLogFatal) {
        return;
    }
    server->logger.log_level = level;
}

void knServer_setLogOutput(
    knServer *server,
    FILE *output
)
{
    if (!server) {
        return;
    }
    server->logger.out = output;
}
