/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** incantation.c
*/

#include "server.h"

static const incantation_requirement_t incantation_requirements[] = {
    {1, 1, {1, 0, 0, 0, 0, 0, 0, 0}},
    {2, 2, {1, 1, 1, 0, 0, 0, 0, 0}},
    {3, 2, {2, 0, 1, 0, 2, 0, 0, 0}},
    {4, 4, {1, 1, 2, 0, 1, 0, 0, 0}},
    {5, 4, {1, 2, 1, 3, 0, 0, 0, 0}},
    {6, 6, {1, 2, 3, 0, 1, 0, 0, 0}},
    {7, 6, {2, 2, 2, 2, 2, 1, 0, 0}},
};

static int sub_count_players_on_tile(user_t *user, incantation_params_t
    *params, int count)
{
    if (user->x == params->x && user->y == params->y &&
        user->level == params->level) {
        params->players[count] = user;
        count++;
    }
    return count;
}

int count_players_on_tile(command_t *c_s, incantation_params_t *params)
{
    int count = 0;
    teams_t *team;
    user_t *user;

    for (team = LIST_FIRST(&c_s->teams_head); team != NULL;
        team = LIST_NEXT(team, entries)) {
        for (user = LIST_FIRST(&team->team_user_head); user != NULL;
            user = LIST_NEXT(user, entries)) {
            count = sub_count_players_on_tile(user, params, count);
        }
    }
    return count;
}

int check_incantation_requirements(command_t *c_s,
    incantation_params_t *params)
{
    incantation_requirement_t requirement =
        incantation_requirements[params->level - 1];

    params->player_count = count_players_on_tile(c_s, params);
    if (params->player_count < requirement.players_required ||
        params->tile->resources.linemate <
        requirement.resources_required.linemate ||
        params->tile->resources.deraumere <
        requirement.resources_required.deraumere ||
        params->tile->resources.sibur <
        requirement.resources_required.sibur ||
        params->tile->resources.mendiane <
        requirement.resources_required.mendiane ||
        params->tile->resources.phiras <
        requirement.resources_required.phiras ||
        params->tile->resources.thystame <
        requirement.resources_required.thystame) {
        return 0;
    }
    return 1;
}

static void consume_incantation_resources(incantation_params_t *params)
{
    incantation_requirement_t requirement =
        incantation_requirements[params->level - 1];

    params->tile->resources.linemate -=
        requirement.resources_required.linemate;
    params->tile->resources.deraumere -=
        requirement.resources_required.deraumere;
    params->tile->resources.sibur -=
        requirement.resources_required.sibur;
    params->tile->resources.mendiane -=
        requirement.resources_required.mendiane;
    params->tile->resources.phiras -=
        requirement.resources_required.phiras;
    params->tile->resources.thystame -=
        requirement.resources_required.thystame;
}

static void elevate_players(incantation_params_t *params, command_t *c_s)
{
    int allocated_players = sizeof(params->players) /
        sizeof(params->players[0]);

    for (int i = 0; i < params->player_count && i < allocated_players; i++) {
        if (params->players[i] != NULL) {
            params->players[i]->level++;
            check_end_game(params, c_s, i, params->players[i]->team_name);
        }
    }
}

static void init_incantation_params(command_t *c_s,
    int sockfd, incantation_params_t *params)
{
    memset(params, 0, sizeof(incantation_params_t));
    params->user = find_user_by_fd(c_s, sockfd);
    params->x = params->user->x;
    params->y = params->user->y;
    params->level = params->user->level;
    params->tile = get_tile_from_coordinates(c_s, params->x, params->y);
}

static void send_gui_message(command_t *c_s, incantation_params_t *params)
{
    int allocated_players = sizeof(params->players) /
        sizeof(params->players[0]);

    snprintf(params->buffer, sizeof(params->buffer), "pic %d %d %d",
        params->x, params->y, params->level);
    for (int i = 0; i < params->player_count && i < allocated_players; i++) {
        if (params->players[i] != NULL) {
            snprintf(params->buffer + strlen(params->buffer),
            sizeof(params->buffer) - strlen(params->buffer), " %d",
            params->players[i]->id);
        }
    }
    send_safe_response(c_s->gui_socket, params->buffer, c_s);
}

static void send_player_message(incantation_params_t *params, command_t *c_s)
{
    for (int i = 0; i < params->player_count; i++)
        send_safe_response(params->user->sockfd, "Elevation underway", c_s);
}

bool check_incantation_is_ready(command_t *c_s, int sockfd)
{
    incantation_params_t params;

    init_incantation_params(c_s, sockfd, &params);
    if (params.user->is_incantating)
        return false;
    if (check_incantation_requirements(c_s, &params)) {
        params.user->is_incantating = true;
        send_gui_message(c_s, &params);
        send_player_message(&params, c_s);
        return true;
    }
    return false;
}

void handle_incantation(command_t *c_s, int sockfd, char *resp)
{
    incantation_params_t params;

    params.user = find_user_by_fd(c_s, sockfd);
    params.x = params.user->x;
    params.y = params.user->y;
    params.level = params.user->level;
    params.tile = get_tile_from_coordinates(c_s, params.x, params.y);
    if (check_incantation_requirements(c_s, &params)) {
        consume_incantation_resources(&params);
        elevate_players(&params, c_s);
        snprintf(params.buffer, sizeof(params.buffer), "pie %d %d ok\n",
            params.x, params.y);
        send_safe_response(c_s->gui_socket, params.buffer, c_s);
    } else {
        snprintf(params.buffer, sizeof(params.buffer), "pie %d %d ko\n",
            params.x, params.y);
        send_safe_response(c_s->gui_socket, params.buffer, c_s);
    }
    params.user->is_incantating = false;
}
