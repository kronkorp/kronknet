/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Accept a new connection
*/
#include "../connection.h"
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/server.h"
#include <stdbool.h>
#include <stddef.h>
#include "../../server/server.h"
#include <unistd.h>

static int __set_non_blocking(
    int fd
)
{
    #ifdef _WIN32
        u_long mode = 1;
        if (ioctlsocket(fd, FIONBIO, &mode) != 0) {
            return -1;
        }
    #else
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags != -1)
            fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        else
            return -1;
    #endif /* _WIN32 */
    return 0;
}

knConnection *knConnection_accept(
    const knServer *server
)
{
    knConnection *conn;
    int fd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    fd = accept(server->fd, (struct sockaddr *)&addr, &len);
    if (fd == -1) {
        return NULL;
    }
    if (__set_non_blocking(fd) == -1) {
        return NULL;
    }
    conn = knConnection_create(&addr, server->flags);
    if (!conn) {
        close(fd);
        return NULL;
    }
    conn->fd = fd;
    return conn;
}
