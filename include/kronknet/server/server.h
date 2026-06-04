/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Server struct definition
*/
#ifndef KRONKNET_SERVER_H
    #define KRONKNET_SERVER_H
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <stdbool.h>
    #include "kronknet/callback/callback.h"
    #include "pool/pool.h"

    #define KNBUFFSIZ  8192

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  server_s
 *
 * @brief   Server structure, containing necessary datas: pollfds, addr, ...
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_server_s {

    bool               running;          //!< Is the server running
    bool               logs;             //!< Is the server should produce logs
    int                fd;               //!< The fd of the server socket
    int                status;           //!< The status of the server, set on error
    struct sockaddr_in addr;             //!< The address of the server
    knPool             pool;             //!< The pool of pollfds to look on
    void              *data;             //!< Data like a struct given by the user
    knConnectionCb     onConnection;     //!< onConnection callback
    knReadCb           onRead;           //!< onRead callback
    knEventCb          onWrite;          //!< onWrite callback
    knConnectionCb     onDisconnection;  //!< onDisconnection callback

} knServer;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documentation
knServer *knServer_create(uint16_t port);
int knServer_init(knServer *server, uint16_t port);

void knServer_clear(knServer *server);
void knServer_destroy(knServer *server);

int knServer_receiveData(knServer *server, knConnection *conn);
int knServer_accept(knServer *server);

int knServer_runOnce(knServer *server, ssize_t timeoutMs);
int knServer_run(knServer *server);

void knServer_kick(knServer *server, knConnection *conn);
void knServer_kickAtIndex(knServer *server, size_t idx);

bool knServer_isRunning(const knServer *server);

void *knServer_getData(const knServer *server);
void knServer_setData(knServer *server, void *data);

void knServer_out(const knServer *server, const char *format, ...);
void knServer_err(const knServer *server, const char *format, ...);
void knServer_setLogging(knServer *server, bool shouldLog);

#endif /* KRONKNET_SERVER_H */
