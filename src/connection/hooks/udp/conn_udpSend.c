/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send message to a connection
*/
#include "../../connection.h"
#include "kronknet/macros/errdef.h"
#include "../../../platform/socket.h"
#include <stddef.h>
#include <stdlib.h>

KN_API
int knConnection_udpSendHook(
    knConnection *conn,
    const void *data,
    size_t size
)
{
    // A datagram is sent whole, now, or not at all. It is never kept in the out buffer to be sent
    // later: the buffer is flushed in a single sendto(), which would glue it to the datagrams kept
    // after it, and the peer would receive them as one. When the socket is full the datagram is
    // dropped: UDP can lose it anyway, and what is built on UDP deals with that.
    ssize_t written = knSocket_sendTo(conn->fd, data, size, &conn->addr);

    if (written == -1 && !knSocket_wouldBlock()) {
        return KNEVTKICK;
    }
    // (Nothing is kept, so there is nothing to wait for the socket to be writable for: KN_POLLOUT is not armed)
    return KNEVTOK;
}
