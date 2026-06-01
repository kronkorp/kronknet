/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Destroy a connection
*/
#include "kronknet/connection/connection.h"
#include <stdlib.h>

void *knConnection_getData(knConnection *conn)
{
    if (!conn) {
        return NULL;
    }
    return conn->data;
}
