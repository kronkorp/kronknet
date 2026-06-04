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
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct kronknet_client_s
 *
 * @brief  Client structure, containing necessary data to the client: fd, data
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_client_s {

    bool     shouldLog;       //!< Should the client produce log ?
    bool     running;         //!< Is the client running ??
    int      fd;              //!< The file descriptor of the client
    void    *data;            //!< The user data ?
    knRBuff *buff;            //!< The out buffer (when send is not possible)
    short    events;          //!< The events
    struct sockaddr_in addr;  //!< The address of the client
    knClientCb         onConnection;
    knClientReadCb     onRead;
    knClientCb         onWrite;
    knClientCb         onDisconnection;

} knClient;
///////////////////////////////////////////////////////////////////////////////

// TODO: Do doc
knClient *knClient_create(void);
int knClient_init(knClient *client);
void knClient_clear(knClient *client);
void knClient_destroy(knClient *client);

int knClient_runOnce(knClient *client, ssize_t timeout);
int knClient_run(knClient *client);
int knClient_receiveData(knClient *client);

void knClient_out(const knClient *client, const char *format, ...);
void knClient_err(const knClient *client, const char *format, ...);
void knClient_setLogging(knClient *server, bool shouldLog);

int knClient_sendServer(knClient *client, void *data, size_t size);

int knClient_connect(knClient *client, const char *ip, uint16_t port);

bool knClient_isRunning(knClient *client);

#endif /* KRONKNET_CLIENT_H */
