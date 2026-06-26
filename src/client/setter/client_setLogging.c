/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Get logging data
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "../client.h"
#include "kronknet/macros/types.h"
#include <stdio.h>

void knClient_setLogLevel(
    knClient *client,
    knLogLevel level
)
{
    if (!client) {
        return;
    }
    client->logger.log_level = level;
}

void knClient_setLogOutput(
    knClient *client,
    FILE *output
)
{
    if (!client) {
        return;
    }
    client->logger.out = output;
}
