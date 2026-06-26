/*
** EPITECH PROJECT, 2026
** KRONKNET
** File description:
** Run the client
*/
#include "kronknet/callback/callback.h"
#include "kronknet/client/client.h"
#include "../client.h"
#include "kronknet/macros/types.h"

knBool knClient_isRunning(
    knClient *client
)
{
    return (client && client->running);
}
