/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Make an address of an IP written as text
*/
#include "kronknet/macros/errdef.h"
#include <string.h>
#include "address.h"

int knAddr_fromIp(
    knAddr *addr,
    const char *ip,
    knPort port
)
{
    struct addrinfo hints = {0};
    struct addrinfo *found = NULL;
    int status = KNEVTARGS;

    // NOTE: AI_NUMERICHOST: getaddrinfo() only reads the IP, it never asks a DNS.
    //       Unlike inet_pton(), it knows the zone of an IPv6 link-local address (fe80::1%eth0)
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_NUMERICHOST;
    if (!ip || getaddrinfo(ip, NULL, &hints, &found) != 0) {
        return KNEVTARGS;
    }
    memset(addr, 0, sizeof(*addr));
    if (found->ai_family == AF_INET || found->ai_family == AF_INET6) {
        memcpy(&addr->any, found->ai_addr, (size_t)found->ai_addrlen);
        addr->len = (socklen_t)found->ai_addrlen;
        if (found->ai_family == AF_INET6) {
            addr->v6.sin6_port = htons(port);
        } else {
            addr->v4.sin_port = htons(port);
        }
        status = KNEVTOK;
    }
    freeaddrinfo(found);
    return status;
}
