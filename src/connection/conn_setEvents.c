/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Send message to a connection
*/
#include "kronknet/connection/connection.h"
#include "kronknet/errdef.h"

int knConnection_setEvents(knConnection *conn, short int events)
{
    if (!conn) {
        return KNEVTARGS;
    }
    *(conn->evtptr) = events;
    return KNEVTOK;
}
