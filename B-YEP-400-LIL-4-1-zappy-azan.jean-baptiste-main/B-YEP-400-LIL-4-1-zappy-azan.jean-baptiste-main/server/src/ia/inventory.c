/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

void inventory(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);
    char buffer[256];

    (void) resp;
    if (user == NULL)
        return;
    snprintf(buffer, sizeof(buffer), "[ food %d, linemate %d, deraumere %d, "
        "sibur %d, mendiane %d, phiras %d, thystame %d ]",
        user->inventory->food, user->inventory->linemate,
        user->inventory->deraumere, user->inventory->sibur,
        user->inventory->mendiane, user->inventory->phiras,
        user->inventory->thystame);
    send_safe_response(sockfd, buffer, c_s);
}
