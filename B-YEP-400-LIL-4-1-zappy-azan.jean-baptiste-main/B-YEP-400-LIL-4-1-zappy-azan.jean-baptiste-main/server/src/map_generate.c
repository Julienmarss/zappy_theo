/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** map_generate.c
*/
#include "server.h"

int sub_my_case(const char *resource, tile_t *tile)
{
    if (strcmp(resource, "sibur") == 0 && tile->resources.sibur == 0) {
        tile->resources.sibur++;
        return 1;
    }
    if (strcmp(resource, "mendiane") == 0 && tile->resources.mendiane == 0) {
        tile->resources.mendiane++;
        return 1;
    }
    if (strcmp(resource, "phiras") == 0 && tile->resources.phiras == 0) {
        tile->resources.phiras++;
        return 1;
    }
    if (strcmp(resource, "thystame") == 0 && tile->resources.thystame == 0) {
        tile->resources.thystame++;
        return 1;
    }
    return 0;
}

int my_switch_case(const char *resource, tile_t *tile)
{
    if (strcmp(resource, "food") == 0 && tile->resources.food == 0) {
        tile->resources.food++;
        return 1;
    }
    if (strcmp(resource, "linemate") == 0 && tile->resources.linemate == 0) {
        tile->resources.linemate++;
        return 1;
    }
    if (strcmp(resource, "deraumere") == 0 && tile->resources.deraumere == 0) {
        tile->resources.deraumere++;
        return 1;
    }
    return 0;
}

void distribute_resources(map_t *map, float count, const char *resource)
{
    int x = 0;
    int y = 0;
    line_t *line = NULL;
    tile_t *tile = NULL;

    while (count > 0) {
        x = rand() % map->width;
        y = rand() % map->height;
        line = TAILQ_FIRST(&map->lines);
        for (int i = 0; i < y; i++)
            line = TAILQ_NEXT(line, lines);
        tile = TAILQ_FIRST(&line->tiles);
        for (int i = 0; i < x; i++)
            tile = TAILQ_NEXT(tile, tiles);
        if (my_switch_case(resource, tile) == 0 &&
            sub_my_case(resource, tile) == 0)
            continue;
        count--;
    }
}

static void generate_resources(map_t *map)
{
    int total_tiles = map->width * map->height;
    float food_count = total_tiles * 0.5;
    float linemate_count = total_tiles * 0.3;
    float deraumere_count = total_tiles * 0.15;
    float sibur_count = total_tiles * 0.1;
    float mendiane_count = total_tiles * 0.1;
    float phiras_count = total_tiles * 0.08;
    float thystame_count = total_tiles * 0.05;

    distribute_resources(map, food_count, "food");
    distribute_resources(map, linemate_count, "linemate");
    distribute_resources(map, deraumere_count, "deraumere");
    distribute_resources(map, sibur_count, "sibur");
    distribute_resources(map, mendiane_count, "mendiane");
    distribute_resources(map, phiras_count, "phiras");
    distribute_resources(map, thystame_count, "thystame");
}

map_t *init_map(int width, int height)
{
    map_t *map = malloc(sizeof(map_t));
    line_t *line = NULL;
    tile_t *tile = NULL;

    map->width = width;
    map->height = height;
    TAILQ_INIT(&map->lines);
    for (int i = 0; i < height; i++) {
        line = create_line();
        for (int j = 0; j < width; j++) {
            tile = create_tile();
            TAILQ_INSERT_TAIL(&line->tiles, tile, tiles);
        }
        TAILQ_INSERT_TAIL(&map->lines, line, lines);
    }
    generate_resources(map);
    return map;
}
