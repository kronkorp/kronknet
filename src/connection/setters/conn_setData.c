/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set a connection data
*/
#include "kronknet/connection/connection.h"

void knConnection_setUserPtr(knConnection *conn, void *data)
{
    if (!conn) {
        return;
    }
    conn->data = data;
}
