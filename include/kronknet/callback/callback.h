/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Struct callback and related functions definition
*/
#ifndef KRONKNET_CALLBACK_H
    #define KRONKNET_CALLBACK_H
    #include <stddef.h>

typedef struct kronknet_server_s     knServer;
typedef struct kronknet_client_s     knClient;
typedef struct kronknet_connection_s knConnection;

typedef int (*knServer_OnConnect_t)(knServer *server, knConnection *conn);
typedef int (*knServer_OnDisconnect_t)(knServer *server, knConnection *conn);
typedef int (*knServer_OnRead_t)(knConnection *conn, const void *data, size_t size);
typedef int (*knServer_OnWrite_t)(knConnection *connection);

typedef int (*knClient_OnConnect_t)(knClient *client);
typedef int (*knClient_OnDisconnect_t)(knClient *client);
typedef int (*knClient_OnRead_t)(knClient *client, const void *data, size_t size);
typedef int (*knClient_OnWrite_t)(knClient *client);

#endif /* KRONKNET_CALLBACK_H */
