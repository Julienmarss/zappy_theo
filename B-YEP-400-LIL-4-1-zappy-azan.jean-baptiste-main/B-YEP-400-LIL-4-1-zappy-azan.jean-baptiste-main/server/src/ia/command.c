/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

int find_freq_command(command_t *c_s, char *response,
    int sockfd, queue_t *queue)
{
    int freq[12] = {7, 7, 7, 7, 1, 7,
                    0, 42, 7, 7, 7, 300};
    char *flags[12] = {
        "Forward\n", "Right\n", "Left\n", "Look\n",
        "Inventory\n", "Broadcast", "Connect_nbr\n",
        "Fork\n", "Eject\n", "Take", "Set",
        "Incantation\n"
    };
    int flag_len = 0;

    for (int n = 0; n < 12; n++) {
        flag_len = strlen(flags[n]);
        if (strncmp(response, flags[n], flag_len) == 0) {
            queue->freq = freq[n] / c_s->freq;
            return 0;
        }
    }
    return 1;
}

int set_res_from_direction(int base_direction, int direction, int res)
{
    if (base_direction == 0)
        return 0;
    if (direction == 1)
        return base_direction;
    if (direction == 2) {
        res = base_direction + 2;
        if (res > 8)
            res -= 8;
    }
    if (direction == 3) {
        res = base_direction + 4;
        if (res > 8)
            res -= 8;
    }
    if (direction == 4) {
        res = base_direction - 2;
        if (res < 1)
            res += 8;
    }
    return res;
}

int calculate_sound_direction(command_t *c_s, user_t *sender, user_t *receiver)
{
    int map_width = c_s->width;
    int map_height = c_s->height;
    int dx = sender->x - receiver->x;
    int dy = sender->y - receiver->y;
    int direction = receiver->direction;
    int base_direction;
    int res;

    dx = get_delta_x(dx, map_width);
    dy = get_delta_y(dy, map_height);
    base_direction = get_base_direction(dx, dy, 0);
    res = set_res_from_direction(base_direction, direction, 0);
    return res;
}

void broadcast(command_t *c_s, int sockfd, char *resp)
{
    user_t *sender = find_user_by_fd(c_s, sockfd);
    teams_t *team;
    user_t *user;
    char buffer[256];
    int direction;

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        LIST_FOREACH(user, &team->team_user_head, entries) {
            if (user->sockfd != sockfd) {
                direction = calculate_sound_direction(c_s, sender, user);
                snprintf(buffer, sizeof(buffer),
                    "message %d, %s", direction, resp);
                send_safe_response(user->sockfd, buffer, c_s);
            }
        }
    }
    send_safe_response(sockfd, "ok", c_s);
    snprintf(buffer, sizeof(buffer), "pbc %d %s", sender->id, resp);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

teams_t *find_team_by_fd(command_t *c_s, int sockfd) {
    teams_t *team;
    user_t *user;

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        LIST_FOREACH(user, &team->team_user_head, entries) {
            if (user->sockfd == sockfd) {
                return team;
            }
        }
    }
    return NULL;
}

static void connected_player(command_t *c_s, int sockfd, char *resp)
{
    teams_t *team = find_team_by_fd(c_s, sockfd);
    if (team == NULL) {
        send_safe_response(sockfd, "ko", c_s);
        return;
    }
    int unused_slots = team->max_clients - team->current_clients;
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%d", unused_slots);
    send_safe_response(sockfd, buffer, c_s);
}

void handle_comand(command_t *c_s, char *response, int sockfd)
{
    char *tmp;
    void (*funct[12])(command_t *, int, char *) = { &mouvement_f, &mouvement_r,
        &mouvement_l, &vision, &inventory, &broadcast, &connected_player,
        &fork_p, &eject, &take_object, &set_object, &handle_incantation
    };
    char *flags[12] = { "Forward\n", "Right\n", "Left\n",
        "Look\n", "Inventory\n", "Broadcast", "Connect_nbr\n",
        "Fork\n", "Eject\n", "Take", "Set", "Incantation\n"
    };
    int flag_len;

    for (int n = 0; n < 12; n++) {
        flag_len = strlen(flags[n]);
        if (strncmp(response, flags[n], flag_len) == 0) {
            tmp = strchr(response, ' ') + 1;
            funct[n](c_s, sockfd, tmp);
            return;
        }
    }
    send_safe_response(sockfd, "ko", c_s);
}

