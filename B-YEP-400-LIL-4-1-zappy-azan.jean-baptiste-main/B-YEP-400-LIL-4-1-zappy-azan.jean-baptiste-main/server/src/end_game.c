/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** end_game.c
*/

#include "server.h"

void check_end_game(incantation_params_t *params, command_t *c_s, int i,
    char *winner)
{
    teams_t *team;
    user_t *user;
    char *buffer = malloc(sizeof(char) * 1024);
    char *result = malloc(sizeof(char) * 1024);

    if (params->players[i]->level != 8)
        return;
    strcpy(buffer, "seg ");
    strcat(buffer, winner);
    result = strdup(buffer);
    LIST_FOREACH(team, &c_s->teams_head, entries) {
        LIST_FOREACH(user, &team->team_user_head, entries) {
            send_safe_response(user->sockfd, "Game finished", c_s);
            printf("Game finished : Team %s won\n", winner);
            send_safe_response(c_s->gui_socket, result, c_s);
            close(user->sockfd);
        }
    }
    c_s->finish = 0;
}
