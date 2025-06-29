/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** config_socket.c
*/

#include "socket.h"

int config_socket(int port, command_t *c_s)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
        return -1;
    c_s->serv_addr.sin_port = htons(port);
    c_s->serv_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(c_s->serv_addr.sin_zero), '\0', 8);
    if (bind(sockfd, (struct sockaddr *) &c_s->serv_addr,
        sizeof(c_s->serv_addr)) == -1) {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }
    if (listen(sockfd, 10) == -1) {
        perror("Listen failed");
        close(sockfd);
        return -1;
    }
    return sockfd;
}

void init_server(command_t *c_s)
{
    FD_ZERO(&c_s->master_fds);
    FD_ZERO(&c_s->read_fds);
    FD_ZERO(&c_s->write_fds);
    FD_SET(c_s->serv_sockfd, &c_s->master_fds);
    c_s->fdmax = c_s->serv_sockfd;
    c_s->unit_of_time = 0;
    c_s->timeout.tv_usec = 1000000 / c_s->freq;
    c_s->timeout.tv_sec = 0;
    c_s->finish = 1;
}

char *remove_backslash_n(char *str)
{
    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
    return str;
}
