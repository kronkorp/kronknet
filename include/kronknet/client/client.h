/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Client struct definition
*/
#ifndef KRONKNET_CLIENT_H
    #define KRONKNET_CLIENT_H
    #include "kronknet/callback/callback.h"
#include "kronknet/utils/rbuff/rbuff.h"
#include <netinet/in.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct kronknet_client_s
 *
 * @brief  Client structure, containing necessary data to the client: fd, data
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_client_s {

    int      fd;              //!< The file descriptor of the client
    void    *data;            //!< The user data ?
    knRBuff *buff;            //!< The out buffer (when send is not possible)
    struct sockaddr_in addr;  //!< The address of the client
    knClientCb         onConnection;
    knClientReadCb     onRead;
    knClientCb         onWrite;
    knClientCb         onDisconnection;

} knClient;
///////////////////////////////////////////////////////////////////////////////

// TODO: Do doc
knClient *knClient_create(const char *ip, uint16_t port);
int knClient_init(knClient *client, const char *ip, uint16_t port);
void knClient_clear(knClient *client);
void knClient_destroy(knClient *client);

#endif /* KRONKNET_CLIENT_H */
