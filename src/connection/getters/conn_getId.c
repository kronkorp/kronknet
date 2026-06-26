/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Get id of a connection
*/
#include "kronknet/connection/connection.h"
#include <stddef.h>

size_t knConnection_getId(const knConnection *conn)
{
    if (!conn) {
        return -1UL;
    }
    return conn->id;
}
