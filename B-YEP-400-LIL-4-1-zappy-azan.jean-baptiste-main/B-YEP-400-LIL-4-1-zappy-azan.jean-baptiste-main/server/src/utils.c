/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** utils.c
*/

#include "server.h"

void send_safe_response(int fd, char *response, command_t *c_s)
{
    if (fcntl(fd, F_GETFD) != -1) {
        dprintf(fd, "%s\n", response);
    }
}

user_t *find_user_by_fd(command_t *c_s, int sockfd)
{
    teams_t *team = NULL;
    user_t *user = NULL;

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        LIST_FOREACH(user, &team->team_user_head, entries) {
            if (sockfd == user->sockfd) {
                return user;
            }
        }
    }
    return NULL;
}

teams_t *find_team(command_t *c_s, char *team_name)
{
    teams_t *team;

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        if (strcmp(team->team_name, team_name) == 0) {
            return team;
        }
    }
    return NULL;
}

user_t *find_user_by_id(command_t *c_s, int id)
{
    teams_t *team;
    user_t *user;

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        LIST_FOREACH(user, &team->team_user_head, entries) {
            if (user->id == id) {
                return user;
            }
        }
    }
    return NULL;
}
