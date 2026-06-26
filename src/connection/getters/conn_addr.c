/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Get the addr (ip | port) of the connection
*/
#include "kronknet/connection/connection.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <stdint.h>

const char *knConnection_getIp(const knConnection *conn)
{
    if (!conn) {
        return NULL;
    }
    return conn->ip;
}

uint16_t knConnection_getPort(const knConnection *conn)
{
    if (!conn) {
        return 0;
    }
    return conn->port;
}
