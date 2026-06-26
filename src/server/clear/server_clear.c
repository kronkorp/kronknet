/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Clear the server
*/
#include "kronknet/server/server.h"
#include <unistd.h>
#include "../pool/pool.h"
#include "../server.h"

void knServer_clear(
    knServer *server
)
{
    if (!server) {
        return;
    }
    if (server->fd != -1) {
        close(server->fd);
    }
    knPool_clear(&server->pool);
}
