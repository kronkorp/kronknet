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

typedef int (*knConnectionCb)(knServer *, knConnection *);
typedef int (*knEventCb)(knConnection *);
typedef int (*knReadCb)(knConnection *, const void *, size_t size);

int knCallback_onConnection(knServer *server, knConnectionCb callback);
int knCallback_onRead(knServer *server, knReadCb callback);
int knCallback_onWrite(knServer *server, knEventCb callback);
int knCallback_onDisconnection(knServer *server, knConnectionCb callback);

#endif /* KRONKNET_CALLBACK_H */
