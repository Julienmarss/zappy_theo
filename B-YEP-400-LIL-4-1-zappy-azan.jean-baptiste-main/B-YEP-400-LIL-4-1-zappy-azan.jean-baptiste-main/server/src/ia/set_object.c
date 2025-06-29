/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

void increment_mendiane(tile_t *tile, user_t *user)
{
    user->inventory->mendiane--;
    tile->resources.mendiane++;
}

int sub_icrement_tile(tile_t *tile, user_t *user, char *resp)
{
    if (user->inventory->mendiane > 0 && strcmp(resp, "mendiane\n") == 0) {
        increment_mendiane(tile, user);
        return 0;
    }
    if (user->inventory->phiras > 0 && strcmp(resp, "phiras\n") == 0) {
        user->inventory->phiras--;
        tile->resources.phiras++;
        return 0;
    }
    if (user->inventory->thystame > 0 && strcmp(resp, "thystame\n") == 0) {
        user->inventory->thystame--;
        tile->resources.thystame++;
        return 0;
    }
    if (user->inventory->sibur > 0 && strcmp(resp, "sibur\n") == 0) {
        user->inventory->sibur--;
        tile->resources.sibur++;
        return 0;
    }
    return 1;
}

int increment_tile(tile_t *tile, user_t *user, char *resp)
{
    if (user->inventory->food > 0 && strcmp(resp, "food\n") == 0) {
        user->inventory->food--;
        tile->resources.food++;
        return 0;
    }
    if (user->inventory->linemate > 0 &&
    strcmp(resp, "linemate\n") == 0) {
        user->inventory->linemate--;
        tile->resources.linemate++;
        return 0;
    }
    if (user->inventory->deraumere > 0 &&
    strcmp(resp, "deraumere\n") == 0) {
        user->inventory->deraumere--;
        tile->resources.deraumere++;
        return 0;
    }
    return 1;
}

void set_object(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);
    tile_t *tile = get_tile_from_coordinates(c_s, user->x, user->y);
    char buffer[1024];
    char send_bct_buffer[1024];

    if (increment_tile(tile, user, resp) == 0 ||
        sub_icrement_tile(tile, user, resp) == 0) {
        snprintf(buffer, sizeof(buffer), "pgt %d %i\n", user->id,
            get_object_id(resp));
        snprintf(send_bct_buffer, sizeof(send_bct_buffer),
            "bct %d %d %d %d %d %d %d %d %d",
            user->x, user->y, tile->resources.food, tile->resources.linemate,
            tile->resources.deraumere, tile->resources.sibur,
            tile->resources.mendiane, tile->resources.phiras,
            tile->resources.thystame);
        send_safe_response(c_s->gui_socket, buffer, c_s);
        send_safe_response(c_s->gui_socket, send_bct_buffer, c_s);
        send_safe_response(sockfd, "ok", c_s);
    } else
        send_safe_response(sockfd, "ko", c_s);
}
