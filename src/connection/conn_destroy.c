/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy a connection
*/
#include "kronknet/connection/connection.h"
#include <stdlib.h>

void knConnection_destroy(knConnection *conn)
{
    if (!conn) {
        return;
    }
    free(conn);
}
