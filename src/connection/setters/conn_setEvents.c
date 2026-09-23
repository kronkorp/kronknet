/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send message to a connection
*/
#include "../connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include <stddef.h>
#include <sys/epoll.h>

int knConnection_setEvents(
    knConnection *conn,
    uint32_t events
)
{
    struct epoll_event ev;

    if (!conn) {
        return KNEVTARGS;
    }
    // NOTE: In UDP, conn->fd is the server socket, so data.ptr stays NULL
    ev.events = events;
    ev.data.ptr = (conn->flags & knUDP) ? NULL : conn;
    if (epoll_ctl(conn->epollfd, EPOLL_CTL_MOD, conn->fd, &ev) == -1) {
        return KNEVTNET;
    }
    return KNEVTOK;
}
