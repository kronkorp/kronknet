/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** network library in C, for the zappy project
*/
#include "kronknet/server/server.h"
#include "kronknet/connection/connection.h"
#include "kronknet/server/callback/callback.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Player_s
{

    knConnection *conn;
    int x;

} Player;

int onConnectionCallback([[maybe_unused]] knServer *server, knConnection *conn)
{
    Player *player = malloc(sizeof(Player));

    if (!player)
        return -1;
    player->conn = conn;
    player->x = rand() % 255;
    knConnection_setData(conn, player);
    printf("[%d] Hello guys from callback!!\n", player->x);
    return 0;
}

int onReadCallback([[maybe_unused]] knConnection *conn, const char *str, size_t n)
{
    Player *player = knConnection_getData(conn);

    if (!player)
        return -1;
    printf("[%d] I received: %*s\n", player->x, (int)n, str);
    return 0;
}

int onDisconnectionCallback([[maybe_unused]] knServer *server, knConnection *conn)
{
    Player *player = knConnection_getData(conn);

    if (!player)
        return -1;
    printf("[%d] Guys i'm tired\n", player->x);
    return 0;
}

int main(void)
{
    knServer *server = knServer_create(4242);

    knServer_setLogging(server, true);
    knCallback_onConnection(server, &onConnectionCallback);
    knCallback_onRead(server, &onReadCallback);
    knCallback_onDisconnection(server, &onDisconnectionCallback);

    // knServer_run(server);
    while (knServer_isRunning(server)) {
        knServer_runOnce(server, 2000);
        printf("Update game loop\n");
    }
    knServer_destroy(server);
    return 0;
}
