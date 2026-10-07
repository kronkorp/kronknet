/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Destroy a connection
*/
#include "../connection.h"
#include "kronknet/connection/connection.h"
#include "kronknet/macros/types.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stdlib.h>
#include "../../platform/socket.h"

void knConnection_destroy(
    knConnection *conn
)
{
    if (!conn) {
        return;
    }
    if (conn->flags & knTCP) {
        knSocket_close(conn->fd);
        conn->fd = KN_INVALID_SOCKET;
    }
    if (conn->out_buff) {
        knRBuff_destroy(conn->out_buff);
    }
    free(conn);
}
