#include "kronknet/errdef.h"
#include "kronknet/server/server.h"
#include "kronknet/connection/connection.h"
#include "kronknet/callback/callback.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_PLAYER 10
#define MAX_ATTEMPTS 6

typedef struct GameState_s GameState;

typedef struct Player_s {
    knConnection *conn;
    int id;
    GameState *game;
} Player;

struct GameState_s {
    Player *players[MAX_PLAYER];
    size_t nplayers;
    int player_id_counter;

    char secret_word[32];
    char display_word[32];
    int attempts_left;
};

void broadcast_message(GameState *game, const char *msg)
{
    size_t len = strlen(msg);
    for (size_t i = 0; i < game->nplayers; i++) {
        knConnection_send(game->players[i]->conn, msg, len);
    }
}

void broadcast_board(GameState *game)
{
    char buffer[256];
    int len = snprintf(buffer, sizeof(buffer),
        "\r\n=========================\r\n"
        "Mot : %s\r\n"
        "Erreurs permises : %d\r\n"
        "Proposez une lettre !\r\n"
        "=========================\r\n> ",
        game->display_word, game->attempts_left);
    
    for (size_t i = 0; i < game->nplayers; i++) {
        knConnection_send(game->players[i]->conn, buffer, len);
    }
}

void reset_game(GameState *game)
{
    strcpy(game->secret_word, "EPITECH");
    strcpy(game->display_word, "_______");
    game->attempts_left = MAX_ATTEMPTS;
    broadcast_message(game, "\r\n\r\n*** NOUVELLE PARTIE ! ***\r\n");
    broadcast_board(game);
}

int onConnectionCallback(knServer *server, knConnection *conn)
{
    GameState *game = knServer_getData(server);
    if (!game || game->nplayers >= MAX_PLAYER) {
        return KNEVTKICK;
    }

    Player *player = malloc(sizeof(Player));
    if (!player) {
        return KNEVTMEM;
    }

    player->conn = conn;
    player->id = game->player_id_counter++;
    player->game = game; 
    knConnection_setData(conn, player);

    game->players[game->nplayers++] = player;

    printf("Joueur %d a rejoint la partie.\n", player->id);
    
    char welcome[128];
    int len = snprintf(welcome, sizeof(welcome), "Bienvenue Joueur %d !\r\n", player->id);
    knConnection_send(conn, welcome, len);
    
    broadcast_board(game);
    
    return KNEVTOK;
}

int onReadCallback(knConnection *conn, const void *str, size_t n)
{
    Player *player = knConnection_getData(conn);
    if (!player || !player->game) return KNEVTKICK;

    GameState *game = player->game;
    const char *input = (const char *)str;
    char guess = 0;

    for (size_t i = 0; i < n; i++) {
        if (isalpha(input[i])) {
            guess = toupper(input[i]);
            break;
        }
    }

    if (guess == 0) return KNEVTOK; 

    bool found = false;
    for (int i = 0; game->secret_word[i] != '\0'; i++) {
        if (game->secret_word[i] == guess && game->display_word[i] == '_') {
            game->display_word[i] = guess;
            found = true;
        }
    }

    char action_msg[128];
    snprintf(action_msg, sizeof(action_msg), "\r\n-> Joueur %d a essaye la lettre '%c'...\r\n", player->id, guess);
    broadcast_message(game, action_msg);

    if (!found) {
        game->attempts_left--;
        broadcast_message(game, "RATE !\r\n");
    } else {
        broadcast_message(game, "BIEN JOUE !\r\n");
    }

    if (strchr(game->display_word, '_') == NULL) {
        broadcast_message(game, "\r\n*** VICTOIRE COLLECTIVE ! Le mot etait trouve ! ***\r\n");
        reset_game(game);
    } 
    else if (game->attempts_left <= 0) {
        char loss_msg[128];
        snprintf(loss_msg, sizeof(loss_msg), "\r\n*** PENDU ! Vous avez perdu. Le mot etait : %s ***\r\n", game->secret_word);
        broadcast_message(game, loss_msg);
        reset_game(game);
    } 
    else {
        broadcast_board(game);
    }

    return KNEVTOK;
}

int onDisconnectionCallback(knServer *server, knConnection *conn)
{
    Player *player = knConnection_getData(conn);
    GameState *game = knServer_getData(server);

    if (!player || !game) return KNEVTOK;

    printf("Joueur %d a quitte la partie.\n", player->id);

    for (size_t i = 0; i < game->nplayers; ++i) {
        if (game->players[i] == player) {
            game->players[i] = game->players[game->nplayers - 1];
            game->players[game->nplayers - 1] = NULL;
            game->nplayers--;
            break;
        }
    }
    free(player);
    return KNEVTOK;
}

int main(void)
{
    knServer *server = knServer_create(4242);
    GameState game = {0};
    
    strcpy(game.secret_word, "EPITECH");
    strcpy(game.display_word, "_______");
    game.attempts_left = MAX_ATTEMPTS;
    game.player_id_counter = 1;

    knServer_setData(server, &game);
    knServer_setLogging(server, true); // Si tu as une fonction de log

    knServer_onConnectionCallback(server, &onConnectionCallback);
    knServer_onReadCallback(server, &onReadCallback);
    knServer_onDisconnectionCallback(server, &onDisconnectionCallback);

    printf("Serveur Pendu demarre sur le port 4242...\n");

    while (knServer_isRunning(server)) {
        knServer_runOnce(server, 50); 
    }

    knServer_destroy(server);
    return 0;
}
