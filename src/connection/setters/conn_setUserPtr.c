/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Set a connection data
*/
#include "../connection.h"
#include "kronknet/connection/connection.h"

void knConnection_setUserPtr(
    knConnection *conn,
    void *user_ptr
)
{
    if (!conn) {
        return;
    }
    conn->user_ptr = user_ptr;
}
