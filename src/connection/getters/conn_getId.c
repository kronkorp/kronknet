/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Get id of a connection
*/
#include "kronknet/connection/connection.h"
#include <stddef.h>
#include "../connection.h"

KN_API
size_t knConnection_getId(
    const knConnection *conn
)
{
    if (!conn) {
        return (size_t)-1;
    }
    return conn->id;
}
