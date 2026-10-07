/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Write the IP of an address as text
*/
#include "kronknet/macros/errdef.h"
#include "address.h"

// NOTE: A dual-stack server sees its IPv4 peers as ::ffff:a.b.c.d: they are written a.b.c.d,
//       as an IPv4-only server would
int knAddr_toIp(
    const knAddr *addr,
    char *ip,
    size_t size
)
{
    const char *written;

    if (knAddr_isMappedV4(addr)) {
        written = inet_ntop(AF_INET, addr->v6.sin6_addr.s6_addr + 12, ip, size);
    } else if (addr->any.sa_family == AF_INET6) {
        written = inet_ntop(AF_INET6, &addr->v6.sin6_addr, ip, size);
    } else {
        written = inet_ntop(AF_INET, &addr->v4.sin_addr, ip, size);
    }
    return written ? KNEVTOK : KNEVTERR;
}
