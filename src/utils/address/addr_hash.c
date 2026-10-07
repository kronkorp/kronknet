/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Make a 64 bits key of an address and its port
*/
#include "kronknet/utils/hashmap/hashmap.h"
#include <string.h>
#include "address.h"

static uint64_t __knAddr_hashV4(
    uint32_t ip,
    uint16_t port
)
{
    return (uint64_t)ip << 32 | (uint64_t)port;
}

// NOTE: The IPv4 peers of a dual-stack server get the same key as on an IPv4-only server.
//       The two halves of an IPv6 are mixed before they are put together: numbered the same
//       way, 2001:db8:0:1::2 and 2001:db8:0:2::1 would get the same key otherwise
uint64_t knAddr_hash(
    const knAddr *addr
)
{
    const uint8_t *bytes = addr->v6.sin6_addr.s6_addr;
    uint64_t halves[2];
    uint32_t ip;

    if (addr->any.sa_family == AF_INET) {
        return __knAddr_hashV4(addr->v4.sin_addr.s_addr, addr->v4.sin_port);
    }
    if (knAddr_isMappedV4(addr)) {
        memcpy(&ip, bytes + 12, sizeof(ip));
        return __knAddr_hashV4(ip, addr->v6.sin6_port);
    }
    memcpy(halves, bytes, sizeof(halves));
    return knMap_basicHash(halves[0]) ^ halves[1]
        ^ ((uint64_t)addr->v6.sin6_scope_id << 16) ^ (uint64_t)addr->v6.sin6_port;
}
