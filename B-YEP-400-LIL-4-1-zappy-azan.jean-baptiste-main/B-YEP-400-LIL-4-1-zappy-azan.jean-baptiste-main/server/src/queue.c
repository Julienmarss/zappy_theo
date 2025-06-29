/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"

void enqueue_command(command_t *c_s, const char *command, int sockfd)
{
    user_t *user = find_user_by_fd(c_s, sockfd);
    queue_t *new_command = NULL;

    if (!user)
        return;
    if (user->nb_command >= 10)
        return;
    new_command = malloc(sizeof(queue_t));
    memset(new_command, 0, sizeof(queue_t));
    if (find_freq_command(c_s, (char *)command, sockfd, new_command) == 1) {
        free(new_command);
        return;
    }
    strncpy(new_command->cmd, command, 2048 - 1);
    new_command->cmd[2048 - 1] = '\0';
    TAILQ_INSERT_TAIL(&user->queueHead, new_command, entries);
    user->nb_command++;
}
