/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Get the ip of the server (but useless asf)
*/
#include "kronknet/callback/callback.h"
#include "kronknet/server/server.h"
#include <stdint.h>
#include <netinet/in.h>

uint16_t knServer_getPort(const knServer *server)
{
    if (!server)
        return NULL;
    return ntohs(server->addr.sin_port);
}
