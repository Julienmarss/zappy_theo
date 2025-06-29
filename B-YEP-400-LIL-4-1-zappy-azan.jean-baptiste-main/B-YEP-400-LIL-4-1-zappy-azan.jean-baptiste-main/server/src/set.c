/*
** EPITECH PROJECT, 2024
** set.c
** File description:
** Helper functions for argument parsing
*/

#include "socket.h"

int set_port(command_t *c_s, char *optarg)
{
    c_s->port = atoi(optarg);
    return 0;
}

int set_width(command_t *c_s, char *optarg)
{
    c_s->width = atoi(optarg);
    return 0;
}

int set_height(command_t *c_s, char *optarg)
{
    c_s->height = atoi(optarg);
    return 0;
}

static int count_teams(int argc, char **argv, int optind)
{
    int count = 0;

    while (optind < argc && argv[optind][0] != '-') {
        count++;
        optind++;
    }
    return count;
}

int set_team_names(command_t *c_s, int argc, char **argv)
{
    teams_t *new_team = NULL;
    teams_t *team = NULL;

    optind--;
    c_s->nb_teams = count_teams(argc, argv, optind);
    c_s->nb_current_user = 0;
    LIST_INIT(&c_s->teams_head);
    for (int i = 0; i < c_s->nb_teams; i++) {
        new_team = malloc(sizeof(teams_t));
        new_team->team_name = strdup(argv[optind]);
        LIST_FOREACH(team, &c_s->teams_head, entries) {
            if (strcmp(team->team_name, new_team->team_name) == 0)
                return -1;
        }
        new_team->current_clients = 0;
        LIST_INIT(&new_team->team_user_head);
        LIST_INSERT_HEAD(&c_s->teams_head, new_team, entries);
        optind++;
    }
    return 0;
}

int set_clients_per_team(command_t *c_s, char *optarg)
{
    teams_t *team = LIST_FIRST(&c_s->teams_head);

    c_s->clients_per_team = atoi(optarg);
    if (c_s->clients_per_team <= 0)
        return -1;
    for (; team != NULL; team = LIST_NEXT(team, entries)) {
        team->max_clients = c_s->clients_per_team;
        team->user_left = c_s->clients_per_team;
    }
    return 0;
}
