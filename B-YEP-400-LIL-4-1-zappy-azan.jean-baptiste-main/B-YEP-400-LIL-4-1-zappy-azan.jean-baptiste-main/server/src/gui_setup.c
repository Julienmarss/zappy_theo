/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** gui_setup.c
*/

#include "server.h"

tile_t *get_tile_from_coordinates(command_t *c_s, int x, int y)
{
    line_t *line = NULL;
    tile_t *tile = NULL;

    if (x == 999999 && y == 999999)
        return NULL;
    x = (x % c_s->width + c_s->width) % c_s->width;
    y = (y % c_s->height + c_s->height) % c_s->height;
    line = TAILQ_FIRST(&c_s->map->lines);
    for (int i = 0; i < y && line != NULL; i++)
        line = TAILQ_NEXT(line, lines);
    if (line == NULL) {
        return NULL;
    }
    tile = TAILQ_FIRST(&line->tiles);
    for (int i = 0; i < x && tile != NULL; i++)
        tile = TAILQ_NEXT(tile, tiles);
    return tile;
}

void create_bct_response(command_t *c_s, tile_t *tile, int x, int y)
{
    char buffer[1024];

    snprintf(buffer, sizeof(buffer),
        "bct %d %d %d %d %d %d %d %d %d",
        x, y,
        tile->resources.food,
        tile->resources.linemate,
        tile->resources.deraumere,
        tile->resources.sibur,
        tile->resources.mendiane,
        tile->resources.phiras,
        tile->resources.thystame);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

void create_tile_response(command_t *c_s, tile_t *tile, int x, int y)
{
    char buffer[1024];

    snprintf(buffer, sizeof(buffer),
        "bct %d %d %d %d %d %d %d %d %d",
        x, y,
        tile->resources.food,
        tile->resources.linemate,
        tile->resources.deraumere,
        tile->resources.sibur,
        tile->resources.mendiane,
        tile->resources.phiras,
        tile->resources.thystame);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}

void handle_line_tiles(command_t *c_s, line_t *line, int y)
{
    tile_t *tile;
    int x = 0;

    tile = TAILQ_FIRST(&line->tiles);
    while (tile != NULL) {
        create_tile_response(c_s, tile, x, y);
        tile = TAILQ_NEXT(tile, tiles);
        x++;
    }
}

void handle_sgt(command_t *c_s)
{
    char buffer[256];

    snprintf(buffer, sizeof(buffer), "sgt %d", c_s->freq);
    send_safe_response(c_s->gui_socket, buffer, c_s);
}
