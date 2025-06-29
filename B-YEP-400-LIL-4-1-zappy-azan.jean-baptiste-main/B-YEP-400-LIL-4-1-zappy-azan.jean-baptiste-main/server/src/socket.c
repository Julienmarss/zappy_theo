/*
** EPITECH PROJECT, 2024
** socket.c
** File description:
** Socket configuration and server functions
*/

#include "socket.h"
#include <server.h>

static void handle_gui_connexion(command_t *c_s, int new_socket)
{
    c_s->gui_socket = new_socket;
    c_s->gui_connected = true;
    printf("GUI client connected on socket %d\n", new_socket);
    send_basique_gui_info(c_s);
}

static void alert_gui_ia_connnexion(command_t *c_s, user_t *new_user)
{
    char tmp[1024];

    if (c_s->gui_connected != 0) {
        snprintf(tmp, sizeof(tmp), "pnw %d %d %d %d %d %s",
            new_user->id, new_user->x, new_user->y, new_user->direction,
            new_user->level, new_user->team_name);
        send_safe_response(c_s->gui_socket, tmp, c_s);
    } else
        printf("GUI not connected\n");
}

static int handle_ia_connexion(char *buffer, command_t *c_s, int new_socket)
{
    user_t *new_user;
    teams_t *team = find_team(c_s, remove_backslash_n(buffer));
    char *team_name = strdup(remove_backslash_n(buffer));

    if (!team || team->user_left == 0) {
        free(team_name);
        close(new_socket);
        return 1;
    }
    team->user_left--;
    c_s->nb_current_user++;
    new_user = create_and_place_player(team, c_s, new_socket);
    snprintf(buffer, sizeof(buffer), "%d\n%d %d\n", team->user_left,
        new_user->x, new_user->y);
    send_safe_response(new_socket, buffer, c_s);
    alert_gui_ia_connnexion(c_s, new_user);
    return 0;
}

bool accept_connexion(command_t *c_s, int new_socket, char *buffer)
{
    if (strcmp(buffer, "GRAPHIC") == 0) {
        handle_gui_connexion(c_s, new_socket);
        return true;
    } else if (handle_ia_connexion(buffer, c_s, new_socket) == 1)
        return false;
    return true;
}

void handle_new_connection(command_t *c_s)
{
    int new_socket = accept(c_s->serv_sockfd,
        (struct sockaddr *)&c_s->client_addr, &c_s->client_len);
    char *buffer = malloc(sizeof(char) * 1024);
    int read_size;

    if (new_socket != -1) {
        send_safe_response(new_socket, "WELCOME", c_s);
        read_size = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
        buffer[read_size] = '\0';
        if (accept_connexion(c_s, new_socket, buffer) == false)
            return;
        FD_SET(new_socket, &c_s->master_fds);
        if (new_socket > c_s->fdmax)
            c_s->fdmax = new_socket;
    } else
        perror("Accept failed");
    free(buffer);
}

void handle_client_data2(command_t *c_s, int i, int nbytes)
{
    char *command = NULL;
    char full_command[2048];

    c_s->buffer[nbytes] = '\0';
    if (i == c_s->gui_socket)
        handle_gui_command(i, c_s, c_s->buffer);
    else {
        command = strtok(c_s->buffer, "\n");
        while (command != NULL) {
            snprintf(full_command, sizeof(full_command), "%s\n", command);
            enqueue_command(c_s, full_command, i);
            command = strtok(NULL, "\n");
        }
    }
}

void handle_client_data(command_t *c_s, int i)
{
    int nbytes = recv(i, c_s->buffer, sizeof(c_s->buffer) - 1, 0);

    if (nbytes <= 0) {
        if (nbytes == 0)
            printf("Socket %d hung up\n", i);
        close(i);
        FD_CLR(i, &c_s->master_fds);
        if (i == c_s->gui_socket) {
            c_s->gui_connected = false;
            c_s->gui_socket = -1;
        }
    } else
        handle_client_data2(c_s, i, nbytes);
}

static void update_timeout(struct timeval *timeout, long remaining_time)
{
    timeout->tv_sec = remaining_time / 1000000;
    timeout->tv_usec = remaining_time % 1000000;
}

static int handle_select(command_t *c_s, struct timeval *timeout)
{
    c_s->read_fds = c_s->master_fds;
    return select(c_s->fdmax + 1, &c_s->read_fds, NULL, NULL, timeout);
}

int start_server(command_t *c_s)
{
    struct timeval last_tick_time;
    struct timeval timeout;
    struct timeval current_time;
    long remaining_time = c_s->timeout.tv_usec;
    int select_result;

    initialize_server(c_s, &last_tick_time);
    while (c_s->finish == 1) {
        update_timeout(&timeout, remaining_time);
        select_result = handle_select(c_s, &timeout);
        if (select_result == -1) {
            perror("select");
            continue;
        }
        gettimeofday(&current_time, NULL);
        handle_tick(c_s, &last_tick_time, &current_time,
        calculate_elapsed_time(&last_tick_time, &current_time));
        process_fds(c_s);
    }
    return 0;
}
