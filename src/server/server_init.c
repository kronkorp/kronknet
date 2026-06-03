/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init the server
*/
#include "kronknet/errdef.h"
#include "kronknet/server/callback/callback.h"
#include "kronknet/server/pool/pool.h"
#include "kronknet/server/server.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

static int __knServer_nonBlocking(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return KNEVTNET;
    }
    flags = flags | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

static int __knServer_bind(knServer *server)
{
    int opt = 1;

    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) == -1) {
        return KNEVTNET;
    }
    if (bind(server->fd, (const struct sockaddr *)&server->addr,
        sizeof(server->addr)) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

static void __knServer_basics(knServer *server)
{
    server->running = true;
    server->logs = false;
    server->onConnection = NULL;
    server->onWrite = NULL;
    server->onRead = NULL;
    server->onDisconnection = NULL;
}

int knServer_init(knServer *server, uint16_t port)
{
    if (!server) {
        return KNEVTERR;
    }
    __knServer_basics(server);
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->fd == -1)
        return KNEVTNET;
    if (__knServer_nonBlocking(server->fd) != KNEVTOK) {
        close(server->fd);
        return KNEVTNET;
    }
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port);
    server->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (__knServer_bind(server) != KNEVTOK) {
        close(server->fd);
        return KNEVTNET;
    }
    if (listen(server->fd, SOMAXCONN) == -1) {
        close(server->fd);
        return KNEVTNET;
    }
    if (knPool_init(&server->pool) != KNEVTOK) {
        close(server->fd);
        return KNEVTERR;
    }
    knPool_registerFd(&server->pool, server->fd, POLLIN);
    return knPool_registerConnection(&server->pool, NULL);
}
