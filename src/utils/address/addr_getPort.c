/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Get the port of an address
*/
#include "address.h"

knPort knAddr_getPort(
    const knAddr *addr
)
{
    if (addr->any.sa_family == AF_INET6) {
        return ntohs(addr->v6.sin6_port);
    }
    return ntohs(addr->v4.sin_port);
}
