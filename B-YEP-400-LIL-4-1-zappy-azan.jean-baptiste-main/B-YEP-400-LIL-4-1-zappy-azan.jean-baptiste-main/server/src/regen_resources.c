/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** regen_resources.c
*/

#include "server.h"
#include "utils.h"

void send_bct_command_to_gui(int x, int y, resource_t resources,
    command_t *c_s)
{
    char buffer[1024];

    snprintf(buffer, sizeof(buffer),
        "bct %d %d %d %d %d %d %d %d %d", x, y,
        resources.food, resources.linemate,
        resources.deraumere, resources.sibur,
        resources.mendiane, resources.phiras,
        resources.thystame);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

void set_regen(regen_src_t *regen_src, map_t *map)
{
    for (line_t *line = TAILQ_FIRST(&map->lines); line != NULL;
        line = TAILQ_NEXT(line, lines)) {
        for (tile_t *tile = TAILQ_FIRST(&line->tiles); tile != NULL;
        tile = TAILQ_NEXT(tile, tiles)) {
            regen_src->current_food_count += tile->resources.food;
            regen_src->current_linemate_count += tile->resources.linemate;
            regen_src->current_deraumere_count += tile->resources.deraumere;
            regen_src->current_sibur_count += tile->resources.sibur;
            regen_src->current_mendiane_count += tile->resources.mendiane;
            regen_src->current_phiras_count += tile->resources.phiras;
            regen_src->current_thystame_count += tile->resources.thystame;
        }
    }
}

void regenerate_resources(map_t *map, command_t *c_s)
{
    int total_tiles = map->width * map->height;
    regen_src_t *regen_src = malloc(sizeof(regen_src_t));

    memset(regen_src, 0, sizeof(regen_src_t));
    set_regen(regen_src, map);
    distribute_resources(map, (total_tiles * 0.5) -
        regen_src->current_food_count, "food");
    distribute_resources(map, total_tiles * 0.3 -
        regen_src->current_linemate_count, "linemate");
    distribute_resources(map, (total_tiles * 0.15) -
        regen_src->current_deraumere_count, "deraumere");
    distribute_resources(map, (total_tiles * 0.1) -
        regen_src->current_sibur_count, "sibur");
    distribute_resources(map, (total_tiles * 0.1) -
        regen_src->current_mendiane_count, "mendiane");
    distribute_resources(map, (total_tiles * 0.08) -
        regen_src->current_phiras_count, "phiras");
    distribute_resources(map, (total_tiles * 0.05) -
        regen_src->current_thystame_count, "thystame");
    handle_mct(c_s);
}
