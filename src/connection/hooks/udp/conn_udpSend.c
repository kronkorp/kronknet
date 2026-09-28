/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send message to a connection
*/
#include "../../connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>

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
    ssize_t written = sendto(conn->fd, data, size, 0, (struct sockaddr *)&conn->addr, conn->addr_length);

    if (written == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        return KNEVTKICK;
    }
    return KNEVTOK;
}
