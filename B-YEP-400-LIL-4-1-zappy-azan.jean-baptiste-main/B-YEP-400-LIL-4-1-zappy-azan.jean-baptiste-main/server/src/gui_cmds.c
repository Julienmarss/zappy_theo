/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** gui_cmds.c
*/
#include <server.h>

command_entry_t command_table[] = {
    {"msz", handle_msz},
    {"mct", handle_mct},
    {"bct", handle_bct},
    {"tna", handle_tna},
    {"ppo", handle_ppo},
    {"plv", handle_plv},
    {"pin", handle_pin},
    {"sgt", handle_sgt},
    {"sst", handle_sst},
    {NULL, NULL}
};

static void handle_sst(command_t *c_s)
{
    int freq;
    char buffer[256];
    int val = sscanf(c_s->buffer, "sst %d", &freq);

    if (val == -1) {
        send_safe_response(c_s->gui_socket, "sbp", c_s);
        return;
    }
    c_s->freq = freq;
    c_s->timeout.tv_usec = 1000000 / c_s->freq;
    snprintf(buffer, sizeof(buffer), "sst %d", freq);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

static void handle_pin(command_t *c_s)
{
    int id;
    user_t *user;
    char buffer[256];

    if (sscanf(c_s->buffer, "pin %d", &id) != 1) {
        send_safe_response(c_s->gui_socket, "sbp", c_s);
        return;
    }
    user = find_user_by_id(c_s, id);
    if (user == NULL) {
        send_safe_response(c_s->gui_socket, "sbp", c_s);
        return;
    }
    snprintf(buffer, sizeof(buffer), "pin %d %d %d %d %d %d %d %d %d %d",
        user->id, user->x, user->y,
        user->inventory->food, user->inventory->linemate,
        user->inventory->deraumere, user->inventory->sibur,
        user->inventory->mendiane, user->inventory->phiras,
        user->inventory->thystame);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

static void handle_msz(command_t *c_s)
{
    char buffer[256];

    snprintf(buffer, sizeof(buffer), "msz %d %d",
        c_s->map->width, c_s->map->height);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

static void handle_bct(command_t *c_s)
{
    int x;
    int y;
    tile_t *tile = NULL;

    if (sscanf(c_s->buffer, "bct %d %d", &x, &y) != 2) {
        send_safe_response(c_s->gui_socket, "sbp", c_s);
        return;
    }
    tile = get_tile_from_coordinates(c_s, x, y);
    if (tile == NULL) {
        send_safe_response(c_s->gui_socket, "sbp", c_s);
        return;
    }
    create_bct_response(c_s, tile, x, y);
}

void handle_mct(command_t *c_s)
{
    line_t *line;
    int y = 0;

    line = TAILQ_FIRST(&c_s->map->lines);
    while (line != NULL) {
        handle_line_tiles(c_s, line, y);
        line = TAILQ_NEXT(line, lines);
        y++;
    }
}

static void handle_tna(command_t *c_s)
{
    teams_t *team;
    char buffer[256];

    LIST_FOREACH(team, &c_s->teams_head, entries) {
        snprintf(buffer, sizeof(buffer), "tna %s", team->team_name);
        send_safe_response(c_s->gui_socket, buffer, c_s);
    }
}

static void handle_ppo(command_t *c_s)
{
    int id;
    user_t *user;
    char buffer[256];

    if (sscanf(c_s->buffer, "ppo %d", &id) != 1) {
        send(c_s->gui_socket, "sbp", 4, 0);
        return;
    }
    user = find_user_by_id(c_s, id);
    if (user == NULL) {
        send(c_s->gui_socket, "sbp\n", 4, 0);
        return;
    }
    snprintf(buffer, sizeof(buffer), "ppo %d %d %d %d",
        user->id, user->x, user->y, user->direction);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

static void handle_plv(command_t *c_s)
{
    int id;
    user_t *user;
    char buffer[256];

    if (sscanf(c_s->buffer, "plv %d", &id) != 1) {
        send_safe_response(c_s->gui_socket, "sbp", c_s);
        return;
    }
    user = find_user_by_id(c_s, id);
    if (user == NULL) {
        send_safe_response(c_s->gui_socket, "sbp", c_s);
        return;
    }
    snprintf(buffer, sizeof(buffer), "plv %d %d", user->id, user->level);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

void handle_gui_command(int client_socket, command_t *c_s, const char *command)
{
    char buffer[256];

    for (int i = 0; command_table[i].command != NULL; i++) {
        if (strncmp(command, command_table[i].command,
            strlen(command_table[i].command)) == 0) {
            command_table[i].handler(c_s);
            return;
        }
    }
    snprintf(buffer, sizeof(buffer), "suc\n");
    send(client_socket, buffer, strlen(buffer), 0);
}

void send_basique_gui_info(command_t *c_s)
{
    handle_msz(c_s);
    handle_sgt(c_s);
    handle_mct(c_s);
    handle_tna(c_s);
}
