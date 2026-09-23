/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server struct definition
*/
#ifndef KRONKNET_SERVER_TCP_H
    #define KRONKNET_SERVER_TCP_H
    #include "kronknet/callback/callback.h"

int knServer_tcpPolloutHook(knServer* server, knConnection *conn);
int knServer_tcpPollinHook(knServer* server, knConnection *conn);
void knServer_tcpCleanupHook(knServer* server);

#endif /* KRONKNET_SERVER_TCP_H */
