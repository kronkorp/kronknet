/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Struct callback and related functions definition
*/
#ifndef KRONKNET_CALLBACK_H
    #define KRONKNET_CALLBACK_H
    #include <stddef.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <stdbool.h>

typedef struct kronknet_server_s     knServer;
typedef struct kronknet_connection_s knConnection;
typedef struct kronknet_client_s     knClient;

typedef int (*knConnectionCb)(knServer *, knConnection *);
typedef int (*knEventCb)(knConnection *);
typedef int (*knReadCb)(knConnection *, const void *, size_t size);

int knServer_onConnectionCallback(knServer *server, knConnectionCb callback);
int knServer_onReadCallback(knServer *server, knReadCb callback);
int knServer_onWriteCallback(knServer *server, knEventCb callback);
int knServer_onDisconnectionCallback(knServer *server, knConnectionCb callback);

// int knClient_onAccept();
// int knClient_onRead();
// int knClient_onQuit();

#endif /* KRONKNET_CALLBACK_H */
