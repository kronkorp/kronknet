/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Init the server
*/
#include "kronknet/callback/callback.h"
#include "../server.h"

void knServer_setUserPtr(
    knServer *server,
    void *user_ptr
)
{
    if (!server)
        return;
    server->user_ptr = user_ptr;
}
