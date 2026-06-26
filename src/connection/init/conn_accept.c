/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Accept a new connection
*/
#include "../connection.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "arpa/inet.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include "../../server/server.h"

knConnection *knConnection_accept(
    const knServer *server
)
{
    knConnection *conn = calloc(1, sizeof(knConnection));
    static size_t id = 0;
    int flags;

    if (!conn)
        return NULL;
    conn->addr_length = sizeof(conn->addr);
    conn->fd = accept(server->fd, (struct sockaddr *)&conn->addr, &conn->addr_length);
    if (conn->fd == -1) {
        knConnection_destroy(conn);
        return NULL;
    }
    flags = fcntl(conn->fd, F_GETFL, 0);
    if (flags != -1)
        fcntl(conn->fd, F_SETFL, flags | O_NONBLOCK);
    conn->out_buff = knRBuff_create(KNBUFFSIZ);
    if (!conn->out_buff) {
        knConnection_destroy(conn);
        return NULL;
    }
    conn->port = ntohs(conn->addr.sin_port);
    conn->id = id++;
    conn->disconnected = false;
    inet_ntop(AF_INET, &conn->addr.sin_addr, conn->ip, INET_ADDRSTRLEN);
    return conn;
}
