/*
** EPITECH PROJECT, 2026
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
    #include <netinet/in.h>
    #include <sys/socket.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  connection_s
 *
 * @brief   Connection structure, store informations for the server and a
 *          generic ptr to connection information (given by user)
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_connection_s {

    int                fd;                  //!< The file descriptor of the client
    knPort             port;                //!< The port using by the connection
    socklen_t          addr_length;         //!< The length of the addr
    struct sockaddr_in addr;                //!< The actual addr
    char               ip[INET_ADDRSTRLEN]; //!< The well-formated ip adrr
    void              *user_ptr;            //!< The user datas (eg User struct ...)
    knRBuff           *out_buff;            //!< The out buffer    
    short int         *evtptr;              //!< The ptr to the events in the pool
    size_t             id;                  //!< The id of the connection ( !same as fd )
    knBool               disconnected;      //!< Is disconnected ?

} knConnection;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documentation
knConnection *knConnection_accept(const knServer *server);
void knConnection_destroy(knConnection *conn);

int knConnection_setEvents(knConnection *conn, short int events);

#endif /* KRONKNET_CONNECTION_IMPL_H */
