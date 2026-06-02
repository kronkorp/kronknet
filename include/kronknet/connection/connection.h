/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Connection struct definition
*/
#ifndef KRONKNET_CONNECTION_H
    #define KRONKNET_CONNECTION_H
    #include <netinet/in.h>
    #include <sys/socket.h>

typedef struct kronknet_server_s knServer;

///////////////////////////////////////////////////////////////////////////////
/**
 * @struct  connection_s
 *
 * @brief   Connection structure, store informations for the server and a
 *          generic ptr to connection information (given by user)
 */
///////////////////////////////////////////////////////////////////////////////
typedef struct kronknet_connection_s {

    int                fd;           //!< The file descriptor of the client
    int                port;         //!< The port using by the connection
    socklen_t          addr_length;  //!< The length of the addr
    struct sockaddr_in addr;         //!< The actual addr

    char               ip[INET_ADDRSTRLEN]; //!< The well-formated ip adrr

    void *data;                      //!< The user datas (eg User struct ...)

} knConnection;
///////////////////////////////////////////////////////////////////////////////

// TODO: Documentation
knConnection *knConnection_accept(const knServer *server);
void knConnection_destroy(knConnection *conn);

void *knConnection_getData(const knConnection *conn);
void knConnection_setData(knConnection *conn, void *data);

#endif /* KRONKNET_CONNECTION_H */
