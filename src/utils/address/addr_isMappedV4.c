/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Is it an IPv4 address that came through an IPv6 socket
*/
#include <string.h>
#include "address.h"

// NOTE: Not IN6_IS_ADDR_V4MAPPED: what it takes is not the same on every platform
knBool knAddr_isMappedV4(
    const knAddr *addr
)
{
    static const uint8_t prefix[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff};

    return addr->any.sa_family == AF_INET6
        && memcmp(addr->v6.sin6_addr.s6_addr, prefix, sizeof(prefix)) == 0;
}
