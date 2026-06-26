/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy a connection
*/
#include "../connection.h"
#include "kronknet/connection/connection.h"
#include <stdlib.h>

void *knConnection_getUserPtr(
    const knConnection *conn
)
{
    if (!conn) {
        return NULL;
    }
    return conn->user_ptr;
}
