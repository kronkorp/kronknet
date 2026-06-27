/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Server struct definition
*/
#ifndef KRONKNET_SERVER_IMPL_H
    #define KRONKNET_SERVER_IMPL_H
    #include "kronknet/callback/callback.h"
    #include "kronknet/macros/types.h"
    #include <stdbool.h>
    #include "pool/pool.h"
    #include "kronknet/macros/types.h"
    #include "../utils/logger/logger.h"

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  server_s
 *
 * @brief   Server structure, containing necessary datas: pollfds, addr, ...
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_server_s {

    knFlags                 flags;                //!< The flags given at creation
    knBool                  running;              //!< Is the server running
    knSocket                fd;                   //!< The fd of the server socket
    struct sockaddr_in      addr;                 //!< The address of the server
    knPool                  pool;                 //!< The pool of pollfds to look on
    void*                   user_ptr;             //!< Data like a struct given by the user
    knServer_OnConnect_t    onConnection;         //!< onConnection callback
    knServer_OnRead_t       onRead;               //!< onRead callback
    knServer_OnWrite_t      onWrite;              //!< onWrite callback
    knServer_OnDisconnect_t onDisconnect;         //!< onDisconnect callback
    char                    ip[INET_ADDRSTRLEN];  //!< The ip as a string
    knLoggerData            logger;               //!< The logger data

} knServer;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documentation
void knServer_kick(knServer *server, knConnection *conn);
void knServer_kickAtIndex(knServer *server, size_t idx);

int knServer_receiveData(knServer *server, knConnection *conn);
int knServer_accept(knServer *server);


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initialize an existing server instance.
 *
 * @param server The server to initialize.
 * @param port   The port to bind to.
 * @param flags  The flags to apply.
 * @return 0 on success, -1 on failure.
 */
///////////////////////////////////////////////////////////////////////////////
int knServer_init(knServer *server, knPort port, knFlags flags);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Release the internal resources owned by a server.
 *
 * @param server The server to clear.
 */
///////////////////////////////////////////////////////////////////////////////
void knServer_clear(knServer *server);
///////////////////////////////////////////////////////////////////////////////

#endif /* KRONKNET_SERVER_IMPL_H */
