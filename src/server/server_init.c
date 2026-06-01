/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init the server
*/
#include "kronknet/server/pool/pool.h"
#include "kronknet/server/server.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/poll.h>
#include <sys/socket.h>

static int knServer_bind(knServer *server)
{
    int opt = 1;

    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) == -1) {
        return -1;
    }
    if (bind(server->fd, (const struct sockaddr *)&server->addr,
        sizeof(server->addr)) == -1) {
        return -1;
    }
    return 0;
}

int knServer_init(knServer *server, size_t port)
{
    if (!server) {
        return -1;
    }
    server->running = true;
    server->logs = false;
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->fd == -1)
        return -1;
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port);
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (knServer_bind(server) == -1)
        return -1;
    if (listen(server->fd, SOMAXCONN) == -1)
        return -1;
    if (knPool_init(&server->pool) == -1)
        return -1;
    knPool_registerFd(&server->pool, server->fd, POLLIN);
    return knPool_registerConnection(&server->pool, NULL);
}
