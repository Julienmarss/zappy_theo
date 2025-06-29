/*
** EPITECH PROJECT, 2024
** main.c
** File description:
** Main server application
*/

#include "socket.h"
#include "server.h"

static void append_resources(char *buffer, resource_t resources)
{
    if (resources.player > 0)
        strcat(buffer, " player");
    if (resources.food > 0)
        strcat(buffer, " food");
    if (resources.linemate > 0)
        strcat(buffer, " linemate");
    if (resources.deraumere > 0)
        strcat(buffer, " deraumere");
    if (resources.sibur > 0)
        strcat(buffer, " sibur");
    if (resources.mendiane > 0)
        strcat(buffer, " mendiane");
    if (resources.phiras > 0)
        strcat(buffer, " phiras");
    if (resources.thystame > 0)
        strcat(buffer, " thystame");
}

static void append_tile_info(char *result, tile_t *tile)
{
    char buffer[256] = {0};

    append_resources(buffer, tile->resources);
    strcat(result, buffer);
}

static void get_coordinates(coordinates_t *coord)
{
    switch (coord->user->direction) {
        case 1:
            coord->x = coord->user->x + coord->offset;
            coord->y = coord->user->y - coord->level;
            break;
        case 2:
            coord->x = coord->user->x + coord->level;
            coord->y = coord->user->y + coord->offset;
            break;
        case 3:
            coord->x = coord->user->x - coord->offset;
            coord->y = coord->user->y + coord->level;
            break;
        case 4:
            coord->x = coord->user->x - coord->level;
            coord->y = coord->user->y - coord->offset;
            break;
    }
}

static void normalize_coordinates(coordinates_t *coord, int width, int height)
{
    if (coord->x < 0)
        coord->x = width - 1;
    if (coord->y < 0)
        coord->y = height - 1;
    if (coord->x >= width)
        coord->x = 0;
    if (coord->y >= height)
        coord->y = 0;
}

static void gather_tile_info(command_t *c_s,
    coordinates_t *coord, char *result)
{
    tile_t *tile = NULL;

    for (int j = -coord->level; j <= coord->level; j++) {
        coord->offset = j;
        get_coordinates(coord);
        normalize_coordinates(coord, c_s->width, c_s->height);
        tile = get_tile_from_coordinates(c_s, coord->x, coord->y);
        append_tile_info(result, tile);
        if (coord->level != coord->user->level || j != coord->level)
            strcat(result, ",");
    }
}

void vision(command_t *c_s, int sockfd, char *resp)
{
    user_t *user = find_user_by_fd(c_s, sockfd);
    char result[2048] = {0};
    coordinates_t coord;

    coord.user = user;
    strcat(result, "[");
    for (int i = 0; i <= user->level; i++) {
        coord.level = i;
        gather_tile_info(c_s, &coord, result);
    }
    if (result[strlen(result) - 1] == ',') {
        result[strlen(result) - 1] = '\0';
    }
    strcat(result, " ]");
    send_safe_response(sockfd, result, c_s);
}
