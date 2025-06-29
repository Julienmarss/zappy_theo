/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** utils_socket.c
*/

#include "server.h"

static void start_server2(command_t *c_s, int i)
{
    if (FD_ISSET(i, &c_s->read_fds)) {
        if (i == c_s->serv_sockfd)
            handle_new_connection(c_s);
        else
            handle_client_data(c_s, i);
    }
}

long calculate_elapsed_time(struct timeval *last_tick_time,
    struct timeval *current_time)
{
    return (current_time->tv_sec - last_tick_time->tv_sec) * 1000000 +
        (current_time->tv_usec - last_tick_time->tv_usec);
}

void handle_tick(command_t *c_s, struct timeval *last_tick_time,
    struct timeval *current_time, long elapsed_time)
{
    long remaining_time = c_s->timeout.tv_usec - elapsed_time;

    if (remaining_time <= 0)
        remaining_time = 0;
    if (elapsed_time >= c_s->timeout.tv_usec && c_s->nb_current_user > 0) {
        tick_queue(c_s);
        *last_tick_time = *current_time;
    }
}

void process_fds(command_t *c_s)
{
    for (int i = 0; i <= c_s->fdmax; i++) {
        if (FD_ISSET(i, &c_s->read_fds))
            start_server2(c_s, i);
    }
}

void initialize_server(command_t *c_s, struct timeval *last_tick_time)
{
    init_server(c_s);
    setup_signal();
    gettimeofday(last_tick_time, NULL);
}
