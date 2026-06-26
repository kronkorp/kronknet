/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Client struct definition
*/
#ifndef KRONKNET_CLIENT_H
    #define KRONKNET_CLIENT_H
    #include "kronknet/callback/callback.h"
    #include "kronknet/macros/optimization.h"
    #include "kronknet/macros/types.h"
    #include <netinet/in.h>
    #include <stdbool.h>
#include <stdio.h>

typedef struct kronknet_client_s knClient;

KN_API knClient *knClient_create(void);
KN_API int knClient_init(knClient *client);
KN_API void knClient_clear(knClient *client);
KN_API void knClient_destroy(knClient *client);
KN_API int knClient_connect(knClient *client, const char *ip, uint16_t port);

KN_API int knClient_runOnce(knClient *client, ssize_t timeout);
KN_API int knClient_run(knClient *client);

KN_API knBool knClient_isRunning(knClient *client);
KN_API void knClient_close(knClient *client);

KN_API int knClient_sendServer(knClient *client, void *data, size_t size);

KN_API void knClient_setUserPtr(knClient *client, void *data);
KN_API void *knClient_getUserPtr(const knClient *client);

KN_API int knClient_setOnConnect(knClient *client, knClient_OnConnect_t callback);
KN_API int knClient_setOnRead(knClient *client, knClient_OnRead_t callback);
KN_API int knClient_setOnWrite(knClient *client, knClient_OnWrite_t callback);
KN_API int knClient_setOnDisconnect(knClient *client, knClient_OnDisconnect_t callback);

KN_API knLogLevel knClient_getLogLevel(const knClient *client);
KN_API FILE* knClient_getLogOutput(const knClient *client);
KN_API void knClient_setLogLevel(knClient *client, knLogLevel level);
KN_API void knClient_setLogOutput(knClient *client, FILE *output);

#endif /* KRONKNET_CLIENT_H */
