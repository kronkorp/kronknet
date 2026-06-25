// KRONKNET - CHAT EXEMPLE (server)
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CHOK   0
#define CHERR 84

// NOTE: OnConnection callback
static int __connectCb(
    knServer *server,
    knConnection *connection
)
{
    printf(
        "New connection from %s:%hu\n",
        knConnection_getIp(connection),
        knConnection_getPort(connection)
    );
    return KNEVTOK;
}


// NOTE: OnDisconnection callback
static int __disconnectCb(
    knServer *server,
    knConnection *connection
)
{
    printf(
        "Lost connection from %s:%hu\n",
        knConnection_getIp(connection),
        knConnection_getPort(connection)
    );
    return KNEVTOK;
}

static int __readCb(
    knConnection *connection,
    const void *data,
    size_t length
)
{
    printf(
        "Received \"%.*s\" from %s:%hu\n",
        (int)length,
        (char *)data,
        knConnection_getIp(connection),
        knConnection_getPort(connection)
    );
    return KNEVTOK;
}

int main(
    int argc,
    char *const *argv
)
{
    if (argc < 2) {
        return CHERR;
    }
    // NOTE: Create server
    knServer *server = knServer_create(strtol(argv[1], NULL, 10));
    if (!server) {
        return CHERR;
    }
    // NOTE: Log server's IP and port
    printf(
        "Server is running on %s:%hu\n",
        knServer_getIp(server),
        knServer_getPort(server)
    );
    // NOTE: Make the server able to log
    knServer_setLogging(server, true);
    knServer_onConnectionCallback(server, &__connectCb);
    knServer_onDisconnectionCallback(server, &__disconnectCb);
    knServer_onReadCallback(server, &__readCb);
    // NOTE: Run the server
    knServer_run(server);
    // NOTE: Destroy server
    knServer_destroy(server);
    return CHOK;
}
