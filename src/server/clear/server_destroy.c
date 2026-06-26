/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy the server
*/
#include "kronknet/server/server.h"
#include <stdlib.h>
#include <unistd.h>

void knServer_destroy(
    knServer *server
)
{
    if (!server) {
        return;
    }
    knServer_clear(server);
    free(server);
}
