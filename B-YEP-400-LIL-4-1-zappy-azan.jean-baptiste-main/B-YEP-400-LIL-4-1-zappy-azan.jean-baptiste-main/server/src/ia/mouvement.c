/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

static void send_ppo_to_gui(command_t *c_s, user_t *user)
{
    char buffer[1024];

    snprintf(buffer, sizeof(buffer), "ppo %i %i %i %i",
        user->id, user->x, user->y, user->direction);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

void mouvement_r(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);

    if (user == NULL)
        return;
    user->direction = (user->direction % 4) + 1;
    send_safe_response(sockfd, "ok", c_s);
    send_ppo_to_gui(c_s, user);
}

void mouvement_l(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);

    if (user == NULL)
        return;
    user->direction = (user->direction - 2 + 4) % 4 + 1;
    send_safe_response(sockfd, "ok", c_s);
    send_ppo_to_gui(c_s, user);
}

static void update_player_position(command_t *c_s, user_t *user)
{
    switch (user->direction) {
        case 1:
            user->y -= 1;
            break;
        case 2:
            user->x += 1;
            break;
        case 3:
            user->y += 1;
            break;
        case 4:
            user->x -= 1;
            break;
    }
}

void mouvement_f(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);
    tile_t *tile;
    tile_t *old_tile = get_tile_from_coordinates(c_s, user->x, user->y);

    if (user == NULL)
        return;
    old_tile->resources.player--;
    update_player_position(c_s, user);
    if (user->x < 0)
        user->x = c_s->width - 1;
    else if (user->x >= c_s->width)
        user->x = 0;
    if (user->y < 0)
        user->y = c_s->height - 1;
    else if (user->y >= c_s->height)
        user->y = 0;
    tile = get_tile_from_coordinates(c_s, user->x, user->y);
    tile->resources.player++;
    send_safe_response(sockfd, "ok", c_s);
    send_ppo_to_gui(c_s, user);
}
