/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy a connection
*/
#include "../connection.h"
#include "kronknet/connection/connection.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stdlib.h>
#include <unistd.h>

void knConnection_destroy(
    knConnection *conn
)
{
    if (!conn) {
        return;
    }
    if (conn->fd != -1) {
        close(conn->fd);
        conn->fd = -1;
    }
    if (conn->out_buff) {
        knRBuff_destroy(conn->out_buff);
    }
    free(conn);
}
