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
    #include <stddef.h>
    #include <stdint.h>
    #include "kronknet/utils/hashmap/hashmap.h"
    #include "pool/pool.h"
    #include "kronknet/macros/types.h"
    #include "../utils/logger/logger.h"

    #define KN_MAX_EVENTS 1024  //!< Max events returned by one epoll_wait

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Declararion of pollin hook
 *
 * @note  conn is the epoll data.ptr: NULL means the server socket.
 *        conn does not serve in UDP mode
 * @return KNEVTKICK if conn has been kicked (and destroyed)
 */
///////////////////////////////////////////////////////////////////////////////
typedef int (*knServer_onPollinHook)(knServer *server, knConnection *conn);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Declaration of pollout hook
 *
 * @note  conn does not serve in UDP mode
 */
///////////////////////////////////////////////////////////////////////////////
typedef int (*knServer_onPolloutHook)(knServer *server, knConnection *conn);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Declaration of Cleanup hook (clear inactive connections)
 */
///////////////////////////////////////////////////////////////////////////////
typedef void (*knServer_onCleanupHook)(knServer *server);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Declaration of Destruction hook
 */
///////////////////////////////////////////////////////////////////////////////
typedef void (*knServer_onDestructionHook)(knServer *server);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  server_s
 *
 * @brief   Server structure, containing necessary datas: epoll pool, addr, ...
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_server_s {

    knFlags                 flags;                //!< The flags given at creation
    knBool                  running;              //!< Is the server running
    knSocket                fd;                   //!< The fd of the server socket
    struct sockaddr_in      addr;                 //!< The address of the server
    knPool                  pool;                 //!< The epoll pool of fds to look on
    void*                   user_ptr;             //!< Data like a struct given by the user

    knServer_OnConnect_t    onConnection;         //!< onConnection callback
    knServer_OnRead_t       onRead;               //!< onRead callback
    knServer_OnWrite_t      onWrite;              //!< onWrite callback
    knServer_OnDisconnect_t onDisconnect;         //!< onDisconnect callback

    char                    ip[INET_ADDRSTRLEN];  //!< The ip as a string

    knLoggerData            logger;               //!< The logger data
    uint64_t                connection_timeout;   //!< The max seconds the connections can rest withoud sending data (default: UDP = 30,000, TCP = 180,000)

    union {

        struct {} on_tcp;  //!< Specific on TCP (maybe later were gonna add smth)
        struct {

            knMap* connections;  //!< The hashmap that contains knConnections

        } on_udp;  //!< Specific on UDP

    };  //!< The union to switch between protocols
    knServer_onPollinHook      onPollinHook;   //!< on pollin hook
    knServer_onPolloutHook     onPolloutHook;  //!< on pollout hook
    knServer_onCleanupHook     onCleanupHook;  //!< on cleanup hook
    knServer_onDestructionHook onDestroyHook;  //!< on destruction hook

} knServer;
///////////////////////////////////////////////////////////////////////////////


// TODO: Documentation

// NOTE: ON TCP
void knServer_kick(knServer *server, knConnection *conn);
void knServer_kickAtIndex(knServer *server, size_t idx);

int knServer_onPollinUDP(knServer *server);

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
