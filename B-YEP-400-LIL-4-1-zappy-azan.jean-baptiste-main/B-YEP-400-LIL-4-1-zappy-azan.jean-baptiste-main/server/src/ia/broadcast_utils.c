/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** broadcast_utils.c
*/

#include "server.h"

void set_base_direction(int *direction, bool *check, int value)
{
    *direction = value;
    *check = true;
}

int get_base_direction(int dx, int dy, int base_direction)
{
    bool check = false;

    if (dx == 0 && dy < 0)
        set_base_direction(&base_direction, &check, 1);
    if (dx < 0 && dy < 0)
        set_base_direction(&base_direction, &check, 2);
    if (dx < 0 && dy == 0)
        set_base_direction(&base_direction, &check, 3);
    if (dx < 0 && dy > 0)
        set_base_direction(&base_direction, &check, 4);
    if (dx == 0 && dy > 0)
        set_base_direction(&base_direction, &check, 5);
    if (dx > 0 && dy > 0)
        set_base_direction(&base_direction, &check, 6);
    if (dx > 0 && dy == 0)
        set_base_direction(&base_direction, &check, 7);
    if (dx > 0 && dy < 0)
        set_base_direction(&base_direction, &check, 8);
    base_direction = !check ? 0 : base_direction;
    return base_direction;
}

int get_delta_x(int dx, int map_w)
{
    if (dx > map_w / 2)
        dx -= map_w;
    if (dx < -map_w / 2)
        dx += map_w;
    return dx;
}

int get_delta_y(int dy, int map_h)
{
    if (dy > map_h / 2)
        dy -= map_h;
    if (dy < -map_h / 2)
        dy += map_h;
    return dy;
}
