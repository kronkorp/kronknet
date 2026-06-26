/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Client struct private impl
*/
#ifndef KRONKNET_CLIENT_IMPL_H
    #define KRONKNET_CLIENT_IMPL_H
    #include "kronknet/callback/callback.h"
    #include "kronknet/macros/types.h"
    #include "kronknet/utils/rbuff/rbuff.h"
    #include <netinet/in.h>
    #include "../utils/logger/logger.h"

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct kronknet_client_s
 *
 * @brief  Client structure, containing necessary data to the client: fd, data
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_client_s {

    knBool                  running;       //!< Is the client running ??
    int                     fd;            //!< The file descriptor of the client
    void*                   user_ptr;      //!< The user data ?
    knRBuff*                buff;          //!< The out buffer (when send is not possible)
    short                   events;        //!< The events
    struct sockaddr_in      addr;          //!< The address of the client
    knClient_OnConnect_t    onConnection;  //!< The callback when the client is connected
    knClient_OnRead_t       onRead;        //!< The callback when the client receive data
    knClient_OnWrite_t      onWrite;       //!< The callback when the client has drained his internal buffer
    knClient_OnDisconnect_t onDisconnect;  //!< The callback when the client disconnect
    knLoggerData            logger;        //!< The logger context

} knClient;
///////////////////////////////////////////////////////////////////////////////

int knClient_receiveData(knClient *client);

#endif /* KRONKNET_CLIENT_IMPL_H */
