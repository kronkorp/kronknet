// KRONKNET - CHAT EXEMPLE (client)
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "kronknet/errdef.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHOK   0
#define CHERR 84

__attribute__((constructor)) static void set_stdin_nonblocking(void)
{
    fcntl(0, F_SETFL, O_NONBLOCK);
}

static int __connectCb(
    knClient *client __attribute__((unused))
)
{
    printf("Connected to server\n");
    return KNEVTOK;
}

static int __readCb(
    knClient *client __attribute__((unused)),
    const void *data,
    size_t length
)
{
    printf(">> %.*s", (int)length, (const char *)data);
    return KNEVTOK;
}

int main(
    int argc,
    char *const *argv
)
{
    if (argc < 3) {
        printf("Bad argument\nUsage: ./ex_chater [ip] [port]\n");
        return CHERR;
    }
    // NOTE: Create client
    knClient *client = knClient_create();
    if (!client) {
        printf("Fail to create client...\n");
        return CHERR;
    }
    // NOTE: Make client able to log
    knClient_setLogging(client, true);
    // NOTE: Set client callbacks
    knClient_onConnectionCallback(client, &__connectCb);
    knClient_onReadCallback(client, &__readCb);
    // NOTE: Connect client to server
    if (knClient_connect(client, argv[1], strtol(argv[2], NULL, 10)) != 0) {
        printf("Unable to connect...\n");
        return CHERR;
    }
    // NOTE: Run the client
    char buff[512];
    while (knClient_isRunning(client)) {
        knClient_runOnce(client, 15);
        if (read(0, buff, sizeof(buff)) > 0) {
            knClient_sendServer(client, buff, strlen(buff));
        }
    }
    // NOTE: Destroy client
    knClient_destroy(client);
    return CHOK;
}
