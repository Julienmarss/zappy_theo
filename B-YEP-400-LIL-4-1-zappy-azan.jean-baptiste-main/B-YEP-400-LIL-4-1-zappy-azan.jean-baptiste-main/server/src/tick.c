/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

void handle_user_commands(command_t *c_s, user_t *user)
{
    queue_t *queue = NULL;

    if (TAILQ_EMPTY(&user->queueHead))
        return;
    queue = TAILQ_FIRST(&user->queueHead);
    if (strncmp(queue->cmd, "Incantation", 11) == 0 && !user->is_incantating) {
        if (check_incantation_is_ready(c_s, user->sockfd)) {
            queue->freq = 300;
        } else
            return;
    }
    if (queue->freq <= 0) {
        handle_comand(c_s, queue->cmd, user->sockfd);
        TAILQ_REMOVE(&user->queueHead, queue, entries);
        free(queue);
        user->nb_command--;
    } else
        queue->freq--;
}

int handle_user_food(command_t *c_s, user_t *user)
{
    if (user->inventory->food <= 0 && user->nb_command > 0) {
        mort(c_s, user);
        return 1;
    }
    if (c_s->unit_of_time >= 126)
        if (user->inventory->food >= 0)
            user->inventory->food--;
    return 0;
}

void handle_team_users(command_t *c_s, teams_t *team)
{
    user_t *user = NULL;

    LIST_FOREACH(user, &team->team_user_head, entries) {
        if (handle_user_food(c_s, user) == 1)
            continue;
        handle_user_commands(c_s, user);
    }
}

void tick_queue(command_t *c_s)
{
    teams_t *team = NULL;

    c_s->unit_of_time++;
    LIST_FOREACH(team, &c_s->teams_head, entries) {
        if (team->user_left == c_s->clients_per_team)
            continue;
        handle_team_users(c_s, team);
    }
    if (c_s->unit_of_time % 20 == 0)
        regenerate_resources(c_s->map, c_s);
    if (c_s->unit_of_time >= 126)
        c_s->unit_of_time = 0;
}
