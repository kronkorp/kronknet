/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Send message to a connection
*/
#include "connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

KN_API
int knConnection_send(
    knConnection *conn,
    const void *data,
    size_t size
)
{
    if (!conn || !data || size == 0) {
        return KNEVTARGS;
    }
    if (conn->sendHook) {
        return conn->sendHook(conn, data, size);
    }
    return KNEVTOK;
}
