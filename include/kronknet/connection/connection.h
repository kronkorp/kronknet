/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Connection struct definition
*/
#ifndef KRONKNET_CONNECTION_H
    #define KRONKNET_CONNECTION_H
    #include "kronknet/macros/optimization.h"
    #include "kronknet/macros/types.h"
    #include <stddef.h>

typedef struct kronknet_connection_s knConnection;

KN_API int knConnection_send(knConnection *conn, const void *data, size_t size);

KN_API void *knConnection_getUserPtr(const knConnection *conn);
KN_API void knConnection_setUserPtr(knConnection *conn, void *data);

KN_API size_t knConnection_getId(const knConnection *conn);

KN_API const char *knConnection_getIp(const knConnection *conn);
KN_API knPort knConnection_getPort(const knConnection *conn);

KN_API void knConnection_disconnect(knConnection *conn);

#endif /* KRONKNET_CONNECTION_H */
