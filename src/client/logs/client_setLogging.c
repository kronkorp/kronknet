/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set the logging, on or off for the client.
*/
#include "kronknet/client/client.h"
#include <stdarg.h>
#include <stdbool.h>

void knClient_setLogging(knClient *server, bool shouldLog)
{
    if (!server) {
        return;
    }
    server->shouldLog = shouldLog;
}
