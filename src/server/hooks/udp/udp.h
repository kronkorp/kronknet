/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server struct definition
*/
#ifndef KRONKNET_SERVER_UDP_H
    #define KRONKNET_SERVER_UDP_H
    #include "kronknet/callback/callback.h"
    #include "../../../platform/socket.h"

int knServer_udpPolloutHook(knServer* server, knConnection *conn);
int knServer_udpPollinHook(knServer* server, knConnection *conn);
void knServer_udpCleanupHook(knServer* server);
void knServer_udpDestroyHook(knServer* server);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Find the connection of a peer
 *
 * @note    on_udp.connections is keyed by knAddr_hash, and two IPv6 peers can
 *          get the same key: the map gives the first connection of that key,
 *          the others follow it through udp_next
 *
 * @return  The connection, or NULL if the peer has none
 */
///////////////////////////////////////////////////////////////////////////////
knConnection *knServer_udpFind(const knServer *server, const knAddr *addr);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Add the connection of a new peer
 *
 * @note    The map does not own the connections: whoever removes one
 *          destroys it (knServer_udpDestroyHook destroys those left)
 *
 * @return  KNEVTOK, or KNEVTMEM
 */
///////////////////////////////////////////////////////////////////////////////
int knServer_udpAdd(knServer *server, knConnection *conn);

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief   Remove a connection (without destroying it)
 *
 * @note    Safe in a knMap_foreach on its key: the map does not move
 */
///////////////////////////////////////////////////////////////////////////////
void knServer_udpRemove(knServer *server, knConnection *conn);

#endif /* KRONKNET_SERVER_UDP_H */
