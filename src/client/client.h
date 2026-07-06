/*
** FREE PROJECT, 2026
** KRONKNET
** File description:
** Client struct private impl
*/
#ifndef KRONKNET_CLIENT_IMPL_H
    #define KRONKNET_CLIENT_IMPL_H
    #include "kronknet/callback/callback.h"
    #include "kronknet/macros/types.h"
    #include "kronknet/utils/rbuff/rbuff.h"
    #include "../utils/logger/logger.h"

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct kronknet_client_s
 *
 * @brief  Client structure, containing necessary data to the client: fd, data
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_client_s {

    knFlags                 flags;         //!< The flags of the client
    knBool                  running;       //!< Is the client running ??
    knSocket                fd;            //!< The file descriptor of the client
    void*                   user_ptr;      //!< The user data ?
    knRBuff*                buff;          //!< The out buffer (when send is not possible)
    short                   events;        //!< The events
    struct sockaddr_in      addr;          //!< The address of the client
    knClient_OnConnect_t    onConnection;  //!< The callback when the client is connected
    knClient_OnRead_t       onRead;        //!< The callback when the client receive data
    knClient_OnWrite_t      onWrite;       //!< The callback when the client has drained his internal buffer
    knClient_OnDisconnect_t onDisconnect;  //!< The callback when the client disconnect
    knLoggerData            logger;        //!< The logger context
    union {
        struct {} on_tcp;
        struct {} on_udp;
    };

} knClient;
///////////////////////////////////////////////////////////////////////////////

int knClient_receiveData(knClient *client);


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initialize a client instance.
 *
 * @param client The client to initialize.
 * @return 0 on success, -1 on failure.
 */
///////////////////////////////////////////////////////////////////////////////
int knClient_init(knClient *client, knFlags flags);
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Release the internal resources owned by a client.
 *
 * @param client The client to clear.
 */
///////////////////////////////////////////////////////////////////////////////
void knClient_clear(knClient *client);
///////////////////////////////////////////////////////////////////////////////

#endif /* KRONKNET_CLIENT_IMPL_H */
