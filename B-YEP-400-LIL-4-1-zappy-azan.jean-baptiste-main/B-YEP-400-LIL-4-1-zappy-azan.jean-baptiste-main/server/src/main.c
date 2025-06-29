/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"
#include <server.h>

int help(void)
{
    printf("USAGE: ./zappy_server -p port -x width ");
    printf("-y height -n name1 name2 ... -c clientsNb -f freq\n");
    printf("\t-p port\t\tport number\n");
    printf("\t-x width\twidth of the world\n");
    printf("\t-y height\theight of the world\n");
    printf("\t-n name1 name2 ...\tnames of the teams\n");
    printf("\t-c clientsNb\tnumber of authorized clients per team\n");
    printf("\t-f freq\treciprocal of time unit for execution of actions\n");
    return 84;
}

int handle_option(command_t *c_s, int opt, char *optarg)
{
    if (opt == 'p')
        return set_port(c_s, optarg);
    if (opt == 'x')
        return set_width(c_s, optarg);
    if (opt == 'y')
        return set_height(c_s, optarg);
    if (opt == 'n')
        return set_team_names(c_s, c_s->argc, c_s->argv);
    if (opt == 'c')
        return set_clients_per_team(c_s, optarg);
    if (opt == 'f'){
        c_s->freq = atoi(optarg);
        if (c_s->freq <= 0)
            return -1;
        return 0;
    }
    return -1;
}

int parse_arguments(int argc, char **argv, command_t *c_s)
{
    int opt;

    c_s->argc = argc;
    c_s->argv = argv;
    for (opt = getopt(argc, argv, "p:x:y:n:c:f:"); opt != -1;
            opt = getopt(argc, argv, "p:x:y:n:c:f:")) {
        if (handle_option(c_s, opt, optarg) == -1) {
            return -1;
        }
    }
    if (c_s->height < 10 || c_s->width < 10 ||
        c_s->height > 30 || c_s->width > 30)
        return -1;
    return 0;
}

command_t init_command_t(void)
{
    command_t c_s = {
            .client_addr = {0},
            .client_len = sizeof(c_s.client_addr),
            .data_socket = 0,
            .serv_addr = {
                .sin_family = AF_INET,
                .sin_addr = {.s_addr = INADDR_ANY}
            },
            .serv_sockfd = 0,
            .gui_connected = false,
            .gui_socket = 0,
    };

    return c_s;
}

int main(int argc, char **argv)
{
    command_t c_s = init_command_t();

    if ((argc == 2 && strcmp(argv[1], "-help") == 0) || argc < 13)
        return help();
    if (parse_arguments(argc, argv, &c_s) != 0)
        return help();
    c_s.map = init_map(c_s.width, c_s.height);
    c_s.serv_sockfd = config_socket(c_s.port, &c_s);
    if (c_s.serv_sockfd != -1)
        start_server(&c_s);
    return 0;
}
