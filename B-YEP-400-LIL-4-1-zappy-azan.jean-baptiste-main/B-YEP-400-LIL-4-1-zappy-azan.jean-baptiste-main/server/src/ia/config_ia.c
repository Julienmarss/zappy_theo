/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

user_t *sub_create_ia(teams_t *team, user_t *user, tile_t *tile, int sockfd)
{
    tile->resources.player++;
    user->direction = rand() % 4 + 1;
    user->level = 1;
    user->sockfd = sockfd;
    team->current_clients++;
    user->id = rand();
    user->inventory->food = 10;
    user->inventory->linemate = 0;
    user->inventory->deraumere = 0;
    user->inventory->sibur = 0;
    user->inventory->mendiane = 0;
    user->inventory->phiras = 0;
    user->inventory->thystame = 0;
    user->loop_cmd = 0;
    user->team_name = team->team_name;
    user->current_command = NULL;
    user->is_incantating = false;
    return user;
}

user_t *create_and_place_player(teams_t *team, command_t *c_s, int sockfd)
{
    user_t *user = malloc(sizeof(user_t));
    tile_t *tile;
    static int fake_pos = 0;

    user->inventory = malloc(sizeof(inventory_t));
    TAILQ_INIT(&user->queueHead);
    TAILQ_INIT(&user->eggHead);
    user->nb_command = 0;
    user->x = rand() % c_s->map->width;
    user->y = rand() % c_s->map->height;
    tile = get_tile_from_coordinates(c_s, user->x, user->y);
    user = sub_create_ia(team, user, tile, sockfd);
    LIST_INSERT_HEAD(&team->team_user_head, user, entries);
    return user;
}
