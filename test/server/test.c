/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** network library in C, for the zappy project
*/
#include "kronknet/errdef.h"
#include "kronknet/mkn/new.h"
#include "kronknet/mkn/object.h"
#include "kronknet/mkn/server.h"
#include "kronknet/server/server.h"
#include "kronknet/connection/connection.h"
#include "kronknet/callback/callback.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYER 4

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
    World  *w = knServer_getData(server);
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
    knConnection_setData(conn, player);
    w->players[w->nplayers] = player;
    w->nplayers++;
    printf("[%d] Hello guys from callback!!\n", player->x);
    return KNEVTOK;
}

int onReadCallback([[maybe_unused]] knConnection *conn, const void *str, size_t n)
{
    Player *player = knConnection_getData(conn);

    if (!player)
        return -1;
    int tmps = n + strlen("Bro really said: ") + 3;
    char tmp[tmps] = {};
    tmp[tmps - 1] = '\0';
    printf("[%d] I received: %.*s\n", player->x, (int)n, (char *)str);
    sprintf(tmp, "Bro really said: %.*s\r\n", (int)tmps, (char *)str);
    knConnection_send(conn, tmp, tmps);
    return 0;
}

int onDisconnectionCallback(knServer *server, knConnection *conn)
{
    Player *player = knConnection_getData(conn);
    World  *w      = knServer_getData(server);

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

int main(void)
{
    // knServer *server = knServer_create(4242);
    World     world = {{}, 0};

    // knServer_setData(server, &world);

    // knServer_setLogging(server, true);
    // knServer_onConnectionCallback(server, &onConnectionCallback);
    // knServer_onReadCallback(server, &onReadCallback);
    // knServer_onDisconnectionCallback(server, &onDisconnectionCallback);

    // knServer_run(server);
    // while (knServer_isRunning(server)) {
    //     knServer_runOnce(server, 2000);
    //     // printf("Update game loop\n");
    //     for (size_t i = 0; i < world.nplayers; ++i) {
    //         printf("Player with fd [%d] is alive!\n", world.players[i]->conn->fd);
    //     }
    // }
    // knServer_destroy(server);

    mknObject *s = new(mknServer, 4242);

    setLogging(s, true);
    setData(s, &world);

    onRead(s, &onReadCallback);
    onConnect(s, &onConnectionCallback);
    onDisconnect(s, &onDisconnectionCallback);

    printf("aha: %s\n", str(s));

    run(s);

    delete(s);
    return 0;
}
