/*
** FREE PROJECT, 2026
** ZAPPY
** File description:
** network library in C, for the zappy project
*/
#include "kronknet/macros/errdef.h"
#include "kronknet/macros/types.h"
#include "kronknet/mkn/server.h"
#include "kronknet/server/server.h"
#include "kronknet/connection/connection.h"
#include "kronknet/callback/callback.h"
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYER 4

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int signum)
{
    if (signum == SIGINT) {
        keep_running = 0;
    }
}

typedef struct Player_s
{

    knConnection *conn;
    int x;

} Player;

typedef struct World_s {

    Player *players[MAX_PLAYER];
    size_t  nplayers;

} World;

int onConnectionCallback([[maybe_unused]] knServer *server, knConnection *conn)
{
    World  *w = knServer_getUserPtr(server);
    Player *player;

    if (!w || w->nplayers >= MAX_PLAYER) {
        return KNEVTKICK;
    }
    player = malloc(sizeof(Player));
    if (!player) {
        return KNEVTMEM;
    }
    player->conn = conn;
    player->x = rand() % 255;
    knConnection_setUserPtr(conn, player);
    w->players[w->nplayers] = player;
    w->nplayers++;
    printf("[%d] Hello guys from callback!!\n", player->x);
    return KNEVTOK;
}

int onReadCallback([[maybe_unused]] knConnection *conn, const void *str, size_t n)
{
    Player *player = knConnection_getUserPtr(conn);

    if (!player)
        return -1;
    int tmps = n + strlen("Bro really said: ") + 3;
    char tmp[tmps] = {};
    memset(tmp, 0, tmps);
    tmp[tmps - 1] = '\0';
    printf("[%d] I received: %.*s\n", player->x, (int)n, (char *)str);
    sprintf(tmp, "Bro really said: %.*s\r\n", (int)tmps, (char *)str);
    knConnection_send(conn, tmp, tmps);
    return 0;
}

int onDisconnectionCallback(knServer *server, knConnection *conn)
{
    Player *player = knConnection_getUserPtr(conn);
    World  *w      = knServer_getUserPtr(server);

    if (!player || !w)
        return -1;
    printf("[%d] Guys i'm tired\n", player->x);
    for (size_t i = 0; i < w->nplayers; ++i) {
        if (w->players[i] == player) {
            w->players[i] = w->players[w->nplayers - 1];
            w->players[w->nplayers - 1] = NULL;
            w->nplayers--;
        }
    }
    free(player);
    return 0;
}

__attribute__((constructor)) void set_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}

int main(void)
{
    knServer *server = knServer_create(4242, knTCP);
    World     world = {{}, 0};

    knServer_setUserPtr(server, &world);

    knServer_setLogLevel(server, knLogTrace);
    knServer_setOnConnect(server, &onConnectionCallback);
    knServer_setOnRead(server, &onReadCallback);
    knServer_setOnDisconnect(server, &onDisconnectionCallback);

    knServer_setLogLevel(server, knLogTrace);
    knServer_setLogOutput(server, stdout);

    knServer_setConnectionTimeout(server, 4000);

    // knServer_run(server);
    while (knServer_isRunning(server) && keep_running) {
        knServer_runOnce(server, 2000);
        // printf("Update game loop\n");
        for (size_t i = 0; i < world.nplayers; ++i) {
            printf("Player with fd [%zu] is alive!\n", knConnection_getId(world.players[i]->conn));
        }
    }
    for (size_t i = 0; i < world.nplayers; ++i) {
        free(world.players[i]);
    }
    knServer_destroy(server);
    return 0;
}
