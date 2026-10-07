/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Are two addresses the same
*/
#include <string.h>
#include "address.h"

// NOTE: Field by field, not the whole struct: what is around the address (padding,
//       sin6_flowinfo) can differ from one datagram to the next of the same peer
knBool knAddr_equal(
    const knAddr *a,
    const knAddr *b
)
{
    if (a->any.sa_family != b->any.sa_family) {
        return knFalse;
    }
    if (a->any.sa_family == AF_INET6) {
        return a->v6.sin6_port == b->v6.sin6_port
            && a->v6.sin6_scope_id == b->v6.sin6_scope_id
            && memcmp(&a->v6.sin6_addr, &b->v6.sin6_addr, sizeof(a->v6.sin6_addr)) == 0;
    }
    return a->v4.sin_port == b->v4.sin_port
        && a->v4.sin_addr.s_addr == b->v4.sin_addr.s_addr;
}
