/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** eject.c
*/
#include "server.h"

void calculate_new_coordinates(int *coordinate, int delta, int max_value)
{
    *coordinate = (*coordinate + delta + max_value) % max_value;
}

void calculate_new_position(position_t *pos)
{
    switch (pos->direction) {
        case 1:
            calculate_new_coordinates(&pos->y, -1, pos->height);
            break;
        case 2:
            calculate_new_coordinates(&pos->x, 1, pos->width);
            break;
        case 3:
            calculate_new_coordinates(&pos->y, 1, pos->height);
            break;
        case 4:
            calculate_new_coordinates(&pos->x, -1, pos->width);
            break;
    }
}

void send_gui_message(command_t *c_s, const char *format, ...)
{
    char buffer[256];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

void handle_eject_player(command_t *c_s, user_t *initiator, user_t *user)
{
    position_t pos = {
        .x = user->x,
        .y = user->y,
        .direction = initiator->direction,
        .width = c_s->map->width,
        .height = c_s->map->height
    };
    char buffer[256];

    calculate_new_position(&pos);
    user->x = pos.x;
    user->y = pos.y;
    snprintf(buffer, sizeof(buffer), "eject: %d", pos.direction);
    send_safe_response(user->sockfd, buffer, c_s);
    snprintf(buffer, sizeof(buffer), "ppo %d %d %d %d", user->id,
        user->x, user->y, user->direction);
    send_gui_message(c_s, buffer);
}

void eject(command_t *c_s, int sockfd, char *resp)
{
    user_t *initiator = find_user_by_fd(c_s, sockfd);
    user_t *user;
    teams_t *team;

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        LIST_FOREACH(user, &team->team_user_head, entries) {
            if (user->x == initiator->x &&
                user->y == initiator->y && user != initiator) {
                handle_eject_player(c_s, initiator, user);
            }
        }
    }
    send_safe_response(sockfd, "ok", c_s);
    send_gui_message(c_s, "pex %d", initiator->id);
}
