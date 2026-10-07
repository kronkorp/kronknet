/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Make the address that binds a port on every interface
*/
#include <string.h>
#include "address.h"

// NOTE: An address of zeros is the "any" of both families: INADDR_ANY, in6addr_any
void knAddr_any(
    knAddr *addr,
    int family,
    knPort port
)
{
    memset(addr, 0, sizeof(*addr));
    if (family == AF_INET6) {
        addr->v6.sin6_family = AF_INET6;
        addr->v6.sin6_port = htons(port);
        addr->len = sizeof(addr->v6);
    } else {
        addr->v4.sin_family = AF_INET;
        addr->v4.sin_port = htons(port);
        addr->len = sizeof(addr->v4);
    }
}
