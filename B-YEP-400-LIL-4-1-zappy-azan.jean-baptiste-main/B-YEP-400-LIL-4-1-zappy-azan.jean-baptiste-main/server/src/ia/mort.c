/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

void mort(command_t *c_s, user_t *user)
{
    tile_t *tile = get_tile_from_coordinates(c_s, user->x, user->y);

    if (tile)
        tile->resources.player--;
    c_s->nb_current_user--;
    send_safe_response(user->sockfd, "dead", c_s);
    FD_CLR(user->sockfd, &c_s->master_fds);
    close(user->sockfd);
    LIST_REMOVE(user, entries);
}
