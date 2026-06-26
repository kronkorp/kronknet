/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Server struct definition
*/
#ifndef KRONKNET_SERVER_H
    #define KRONKNET_SERVER_H
    #include "kronknet/callback/callback.h"
    #include "kronknet/macros/types.h"
    #include "kronknet/macros/optimization.h"
    #include <stdio.h>

typedef struct kronknet_server_s knServer;

// TODO: Documentation
KN_API knServer* knServer_create(knPort port);
KN_API int       knServer_init(knServer *server, uint16_t port);
KN_API void      knServer_clear(knServer *server);
KN_API void      knServer_destroy(knServer *server);

KN_API int knServer_runOnce(knServer *server, ssize_t timeoutMs);
KN_API int knServer_run(knServer *server);

KN_API knBool knServer_isRunning(const knServer *server);
KN_API void knServer_stop(knServer *server);

KN_API void knServer_setLogLevel(knServer *server, knLogLevel level);
KN_API void knServer_setLogOutput(knServer *server, FILE *output);

KN_API FILE*      knServer_getLogOutput(const knServer *server);
KN_API knLogLevel knServer_getLogLevel(const knServer *server);

KN_API void *knServer_getUserPtr(knServer *server);
KN_API void  knServer_setUserPtr(knServer *server, void *data);

KN_API const char* knServer_getIp(const knServer *server);
KN_API knPort      knServer_getPort(const knServer *server);

#endif /* KRONKNET_SERVER_H */
