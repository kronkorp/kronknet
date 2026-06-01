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
    #include "pool/pool.h"

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  server_s
 *
 * @brief   Server structure, containing necessary datas: pollfds, addr, ...
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_server_s {

    bool               running;  //!< Is the server running
    bool               logs;     //!< Is the server should produce logs
    int                status;   //!< The status of the server, set on error
    int                fd;       //!< The fd of the server socket
    struct sockaddr_in addr;     //!< The address of the server
    knPool             pool;     //!< The pool of pollfds to look on

} knServer;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documentation
knServer *knServer_create(size_t port);
int knServer_init(knServer *server, size_t port);

void knServer_clear(knServer *server);
void knServer_destroy(knServer *server);

int knServer_run(knServer *server);

void knServer_out(const knServer *server, const char *format, ...);
void knServer_err(const knServer *server, const char *format, ...);
void knServer_setLogging(knServer *server, bool shouldLog);

#endif /* KRONKNET_SERVER_H */
