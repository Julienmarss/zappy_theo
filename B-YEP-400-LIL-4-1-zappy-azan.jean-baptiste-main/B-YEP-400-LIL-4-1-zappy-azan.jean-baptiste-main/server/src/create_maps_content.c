/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** create_maps_content.c
*/

#include "server.h"

line_t *create_line(void)
{
    line_t *line = malloc(sizeof(line_t));

    if (line == NULL) {
        perror("Failed to allocate line");
        return NULL;
    }
    TAILQ_INIT(&line->tiles);
    return line;
}

tile_t *create_tile(void)
{
    tile_t *tile = malloc(sizeof(tile_t));

    if (tile == NULL) {
        perror("Failed to allocate tile");
        return NULL;
    }
    memset(&(tile->resources), 0, sizeof(resource_t));
    return tile;
}
