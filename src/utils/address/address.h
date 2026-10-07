/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** IPv4 and IPv6 addresses, the same way.
*/
#ifndef KRONKNET_ADDRESS_H
    #define KRONKNET_ADDRESS_H
    #include "kronknet/macros/types.h"
    #include "../../platform/socket.h"
    #include <stddef.h>
    #include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Make the address of an IP written as text, IPv4 or IPv6
 *          ("127.0.0.1", "::1", "fe80::1%eth0")
 *
 * @note    Numbers only: a host name is not resolved (that would block)
 *
 * @return  KNEVTOK, or KNEVTARGS when [ip] is not an IP
 */
///////////////////////////////////////////////////////////////////////////////
int knAddr_fromIp(knAddr *addr, const char *ip, knPort port);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Make the address that binds [port] on every interface
 *          (0.0.0.0 in AF_INET, :: in AF_INET6)
 */
///////////////////////////////////////////////////////////////////////////////
void knAddr_any(knAddr *addr, int family, knPort port);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Get the port of an address
 */
///////////////////////////////////////////////////////////////////////////////
knPort knAddr_getPort(const knAddr *addr);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Is it an IPv4 address that came through an IPv6 socket
 *          (::ffff:a.b.c.d)?
 */
///////////////////////////////////////////////////////////////////////////////
knBool knAddr_isMappedV4(const knAddr *addr);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Write the IP of an address as text
 *
 * @note    An IPv4-mapped address is written as the IPv4 it is (a.b.c.d)
 *
 * @param   ip   Where to write it (INET6_ADDRSTRLEN is always enough)
 * @param   size The size of [ip]
 *
 * @return  KNEVTOK, or KNEVTERR
 */
///////////////////////////////////////////////////////////////////////////////
int knAddr_toIp(const knAddr *addr, char *ip, size_t size);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Are they the same address, with the same port?
 */
///////////////////////////////////////////////////////////////////////////////
knBool knAddr_equal(const knAddr *a, const knAddr *b);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Make a 64 bits key of an address and its port
 *
 * @note    Two IPv4 addresses never get the same key: an IPv4 and its port fit
 *          in 64 bits. Two IPv6 addresses can, so who looks one up by its key
 *          must check it with knAddr_equal
 */
///////////////////////////////////////////////////////////////////////////////
uint64_t knAddr_hash(const knAddr *addr);

#endif /* KRONKNET_ADDRESS_H */
