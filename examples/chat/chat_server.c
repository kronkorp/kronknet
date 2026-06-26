// KRONKNET - CHAT EXEMPLE (server)
#include "kronknet/callback/callback.h"
#include "kronknet/connection/connection.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/server/server.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHOK   0
#define CHERR 84

#define MAXUSR    8
#define MAXMSG 4096

struct usr {

    knConnection* socket;
    char          lastmsg[MAXMSG];
    bool          hasmsg;

};

struct ctx {

    struct usr users[MAXUSR];
    size_t     usrcount;
    knServer*  server;

};

// NOTE: OnConnection callback
static int __connectCb(
    knServer *server,
    knConnection *connection
)
{
    struct ctx* data = (struct ctx*)knServer_getUserPtr(server);
    printf(
        "New connection from %s:%hu\n",
        knConnection_getIp(connection),
        knConnection_getPort(connection)
    );
    if (data->usrcount >= MAXUSR) {
        printf("Server is full, refusing.\n");
        return KNEVTKICK;
    }
    data->users[data->usrcount] = (struct usr){
        .socket = connection,
        .lastmsg = {0},
        .hasmsg = false
    };
    knConnection_setUserPtr(connection, &data->users[data->usrcount]);
    data->usrcount++;
    return KNEVTOK;
}

// NOTE: OnDisconnection callback
static int __disconnectCb(
    knServer *server,
    knConnection *connection
)
{
    struct ctx* data = (struct ctx*)knServer_getUserPtr(server);
    printf(
        "Lost connection from %s:%hu\n",
        knConnection_getIp(connection),
        knConnection_getPort(connection)
    );
    for (size_t i = 0; i < data->usrcount; ++i) {
        if (data->users[i].socket == connection) {
            data->users[i] = data->users[--data->usrcount];
            break;
        }
    }
    return KNEVTOK;
}

static int __readCb(
    knConnection *connection,
    const void *data,
    size_t length
)
{
    struct usr* user = (struct usr*)knConnection_getUserPtr(connection);
    printf(
        "Received \"%.*s\" from %s:%hu\n",
        (int)length,
        (char *)data,
        knConnection_getIp(connection),
        knConnection_getPort(connection)
    );
    strncpy(user->lastmsg, (char *)data, length);
    user->hasmsg = true;
    return KNEVTOK;
}

int main(
    int argc,
    char *const *argv
)
{
    // NOTE: Create context
    struct ctx context = {
        .server = NULL,
        .users = {0},
        .usrcount = 0,
    };
    if (argc < 2) {
        return CHERR;
    }
    // NOTE: Create server
    knServer *server = knServer_create(strtol(argv[1], NULL, 10));
    if (!server) {
        return CHERR;
    }
    // NOTE: Affect knServer to ctx.server
    context.server = server;
    // NOTE: Log server's IP and port
    printf(
        "Server is running on %s:%hu\n",
        knServer_getIp(server),
        knServer_getPort(server)
    );
    // NOTE: Make the server able to log
    knServer_setLogging(server, true);
    // NOTE: Set the context as server data
    knServer_setUserPtr(server, &context);
    // NOTE: Set servers callback
    knServer_onConnectionCallback(server, &__connectCb);
    knServer_onDisconnectionCallback(server, &__disconnectCb);
    knServer_onReadCallback(server, &__readCb);
    // NOTE: Run the server
    while (knServer_isRunning(server)) {
        knServer_runOnce(server, -1);
        // NOTE: Sending messages to all users
        for (size_t i = 0; i < context.usrcount; ++i) {
            if (!context.users[i].hasmsg) continue;
            for (size_t j = 0; j < context.usrcount; ++j) {
                if (context.users[j].socket == context.users[i].socket) continue;
                knConnection_send(context.users[j].socket, context.users[i].lastmsg, strlen(context.users[i].lastmsg));
            }
            context.users->hasmsg = false;
        }
    }
    // NOTE: Destroy server
    knServer_destroy(server);
    return CHOK;
}
