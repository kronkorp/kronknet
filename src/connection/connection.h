/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Connection struct definition
*/
#ifndef KRONKNET_CONNECTION_IMPL_H
    #define KRONKNET_CONNECTION_IMPL_H
    #include "kronknet/callback/callback.h"
    #include "kronknet/macros/types.h"
    #include "kronknet/utils/rbuff/rbuff.h"
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>

typedef int (*knConnection_sendHook)(knConnection *, const void *, size_t);

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  connection_s
 *
 * @brief   Connection structure, store informations for the server and a
 *          generic ptr to connection information (given by user)
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_connection_s {

    knBool                disconnected;        //!< Is disconnected ?
    knSocket              fd;                  //!< The fd of the client (or the server in UDP mode)
    knFlags               flags;               //!< The flags
    size_t                id;                  //!< The id of the connection ( !same as fd )
    socklen_t             addr_length;         //!< The length of the addr
    struct sockaddr_in    addr;                //!< The actual addr
    knPort                port;                //!< The port using by the connection
    char                  ip[INET_ADDRSTRLEN]; //!< The well-formated ip adrr
    uint64_t              last_data;           //!< The last time (ms)
    void                 *user_ptr;            //!< The user datas (eg User struct ...)
    knRBuff              *out_buff;            //!< The out buffer    
    knConnection_sendHook sendHook;            //!< The send hook
    short int            *evtptr;              //!< The ptr to the events in the pool
    union {
        struct {} on_udp;  //!< On UDP datas
        struct {} on_tcp;  //!< On tcp datas
    };

} knConnection;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documentation
knConnection *knConnection_accept(const knServer *server);
void knConnection_destroy(knConnection *conn);

knConnection *knConnection_create(const struct sockaddr_in* addr, knFlags flags);

int knConnection_setEvents(knConnection *conn, short int events);

#endif /* KRONKNET_CONNECTION_IMPL_H */
