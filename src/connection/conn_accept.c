/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Accept a new connection
*/
#include "kronknet/connection/connection.h"
#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "arpa/inet.h"
#include "kronknet/utils/rbuff/rbuff.h"

knConnection *knConnection_accept(const knServer *server)
{
    knConnection *conn = calloc(1, sizeof(knConnection));

    if (!conn) {
        return NULL;
    }
    conn->fd = accept(server->fd, (struct sockaddr *)&conn->addr, &conn->addr_length);
    if (conn->fd == -1) {
        knConnection_destroy(conn);
        return NULL;
    }
    conn->out_buff = knRBuff_create(KNBUFFSIZ);
    if (!conn->out_buff) {
        knConnection_destroy(conn);
        return NULL;
    }
    conn->port = ntohs(conn->addr.sin_port);
    inet_ntop(AF_INET, &conn->addr.sin_addr, conn->ip, INET_ADDRSTRLEN);
    return conn;
}
