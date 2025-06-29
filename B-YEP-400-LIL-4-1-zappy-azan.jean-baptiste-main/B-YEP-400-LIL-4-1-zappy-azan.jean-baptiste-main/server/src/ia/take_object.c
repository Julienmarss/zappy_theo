/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

bool sub_increment_thystame(tile_t *tile, user_t *user,
    char *resp, bool entered)
{
    if (tile->resources.thystame > 0 && strcmp(resp, "thystame\n") == 0) {
        user->inventory->thystame++;
        tile->resources.thystame--;
        entered = true;
    }
    return entered;
}

bool sub_increment_inventory(tile_t *tile, user_t *user,
    char *resp, bool entered)
{
    if (tile->resources.sibur > 0 && strcmp(resp, "sibur\n") == 0) {
        user->inventory->sibur++;
        tile->resources.sibur--;
        entered = true;
    }
    if (tile->resources.mendiane > 0 && strcmp(resp, "mendiane\n") == 0) {
        user->inventory->mendiane++;
        tile->resources.mendiane--;
        entered = true;
    }
    if (tile->resources.phiras > 0 && strcmp(resp, "phiras\n") == 0) {
        user->inventory->phiras++;
        tile->resources.phiras--;
        entered = true;
    }
    return sub_increment_thystame(tile, user, resp, entered);
}

int increment_inventory(tile_t *tile, user_t *user, char *resp)
{
    bool entered = false;

    if (tile->resources.food > 0 && strcmp(resp, "food\n") == 0) {
        user->inventory->food++;
        tile->resources.food--;
        entered = true;
    } else if (tile->resources.linemate > 0 &&
        strcmp(resp, "linemate\n") == 0) {
        user->inventory->linemate++;
        tile->resources.linemate--;
        entered = true;
    }
    if (tile->resources.deraumere > 0 &&
        strcmp(resp, "deraumere\n") == 0) {
        user->inventory->deraumere++;
        tile->resources.deraumere--;
        entered = true;
    }
    entered = sub_increment_inventory(tile, user, resp, entered);
    return entered ? 0 : 1;
}

int get_object_id(char *resp)
{
    if (strcmp(resp, "food\n") == 0)
        return 0;
    if (strcmp(resp, "linemate\n") == 0)
        return 1;
    if (strcmp(resp, "deraumere\n") == 0)
        return 2;
    if (strcmp(resp, "sibur\n") == 0)
        return 3;
    if (strcmp(resp, "mendiane\n") == 0)
        return 4;
    if (strcmp(resp, "phiras\n") == 0)
        return 5;
    if (strcmp(resp, "thystame\n") == 0)
        return 6;
    return -1;
}

void take_object(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);
    tile_t *tile = get_tile_from_coordinates(c_s, user->x, user->y);
    char buffer[1024];
    char send_bct_buffer[1024];

    if (increment_inventory(tile, user, resp) == 0) {
        snprintf(buffer, sizeof(buffer), "pgt %d %i", user->id,
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
