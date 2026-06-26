/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Disconnect a client
*/
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include <stdbool.h>
#include <stddef.h>
#include "../connection.h"

void knConnection_disconnect(
    knConnection *conn
)
{
    if (!conn) {
        return;
    }
    conn->disconnected = true;
}
