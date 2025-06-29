/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** fork.c
*/

#include "server.h"

teams_t *find_team_by_name(command_t *c_s, char *name)
{
    teams_t *team = NULL;

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        if (strcmp(team->team_name, name) == 0)
            return team;
    }
    return NULL;
}

void create_egg(command_t *c_s, user_t *user)
{
    egg_t *new_egg = malloc(sizeof(egg_t));
    char response[128];
    teams_t *team = find_team_by_name(c_s, user->team_name);
    char buffer[256];

    if (!new_egg)
        return;
    new_egg->x = user->x;
    new_egg->y = user->y;
    new_egg->id = 1;
    TAILQ_INSERT_TAIL(&user->eggHead, new_egg, entries);
    snprintf(response, sizeof(response), "ok");
    team->max_clients++;
    send_safe_response(user->sockfd, response, c_s);
    if (c_s->gui_connected) {
        snprintf(buffer, sizeof(buffer), "enw %d %d %d %d\n",
            new_egg->id, user->id, new_egg->x, new_egg->y);
        send_safe_response(c_s->gui_socket, buffer, c_s);
    }
}

void fork_p(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);

    if (!user)
        return;
    create_egg(c_s, user);
}
