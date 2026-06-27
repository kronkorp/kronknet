#include "kronknet/client/client.h"
#include "kronknet/callback/callback.h"
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int onConnectionCallback([[maybe_unused]] knClient *client)
{
    printf("Connected !!\n");
    return KNEVTOK;
}

int onReadCallback([[maybe_unused]] knClient *client, [[maybe_unused]] const void *data, [[maybe_unused]] size_t size)
{
    printf("%.*s\n", (int)size, (char *)data);
    fflush(stdout);
    return KNEVTOK;
}

int onDisconnectionCallback([[maybe_unused]] knClient *client)
{
    printf("\nExiting !!\n");
    return KNEVTOK;
}

void set_stdin_nonblocking(void)
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

int main(void)
{
    knClient *client = knClient_create(knTCP);

    if (!client)
        return 84;

    knClient_setLogLevel(client, knLogTrace);
    knClient_setLogOutput(client, stdout);
    knClient_setOnConnect(client, &onConnectionCallback);
    knClient_setOnRead(client, &onReadCallback);
    knClient_setOnDisconnect(client, &onDisconnectionCallback);

    if (knClient_connect(client, "localhost", 4242) != KNEVTOK) {
        knClient_destroy(client);
        return 84;
    }

    set_stdin_nonblocking();

    char kbd_buffer[512];

    while (knClient_isRunning(client)) {
        
        knClient_runOnce(client, 10);
        memset(kbd_buffer, 0, sizeof(kbd_buffer));
        ssize_t n = read(STDIN_FILENO, kbd_buffer, sizeof(kbd_buffer) - 2);
        if (n > 0) {
            if (kbd_buffer[n - 1] == '\n') {
                kbd_buffer[n - 1] = '\r';
                kbd_buffer[n] = '\n';
                n += 1;
            }
            knClient_sendServer(client, kbd_buffer, n);
        }
    }
    
    knClient_destroy(client);
    return 0;
}
