/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init the server
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/callback/callback.h"
#include "../pool/pool.h"
#include "../server.h"
#include "kronknet/macros/types.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "../pool/pool.h"

static int __knServer_nonBlocking(
    int fd
)
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

static int __knServer_bind(
    knServer *server
)
{
    int opt = 1;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    if (setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt)) == -1) {
        return KNEVTNET;
    }
    if (bind(server->fd, (const struct sockaddr *)&server->addr,
        sizeof(server->addr)) == -1) {
        return KNEVTNET;
    }
    if (getsockname(server->fd, (struct sockaddr *)&addr, &len) == -1) {
        return KNEVTNET;
    }
    if (inet_ntop(AF_INET, &addr.sin_addr, server->ip,
        INET_ADDRSTRLEN) == NULL) {
        return KNEVTNET;
    }
    return KNEVTOK;
}

static void __knServer_basics(knServer *server)
{
    server->running = true;
    server->onConnection = NULL;
    server->onWrite = NULL;
    server->onRead = NULL;
    server->onDisconnect = NULL;
    server->logger = (knLoggerData){
        .out = NULL,
        .log_level = knLogNone,
    };
}

int knServer_init(
    knServer *server,
    knPort port
)
{
    if (!server)
        return KNEVTERR;
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
    return knPool_registerFd(&server->pool, server->fd, NULL, POLLIN);
}
