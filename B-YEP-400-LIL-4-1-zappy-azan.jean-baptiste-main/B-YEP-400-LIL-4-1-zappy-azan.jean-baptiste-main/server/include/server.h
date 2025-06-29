/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** server.h
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/queue.h>
#include <stdbool.h>
#include <stdarg.h>
#include "socket.h"

typedef struct command_t command_t;

typedef struct user_t user_t;

typedef void (*command_handler_t)(command_t *c_s);

// data structures to store and handle the map
typedef struct resource_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
    int player;
} resource_t;

typedef struct ressources_to_add_s {
    int current_food_count;
    int current_linemate_count;
    int current_deraumere_count;
    int current_sibur_count;
    int current_mendiane_count;
    int current_phiras_count;
    int current_thystame_count;
} ressources_to_add_t;

typedef struct tile_s {
    resource_t resources;
    TAILQ_ENTRY(tile_s) tiles;
} tile_t;

typedef struct line_s {
    TAILQ_HEAD(tile_list_s, tile_s) tiles;
    TAILQ_ENTRY(line_s) lines;
} line_t;

typedef struct map_s {
    TAILQ_HEAD(line_list_s, line_s) lines;
    int width;
    int height;
} map_t;

//function pointer to handle commands
typedef struct {
    const char *command;
    command_handler_t handler;
} command_entry_t;

//structure to store the incantation requirements
typedef struct {
    int level;
    int players_required;
    resource_t resources_required;
} incantation_requirement_t;

//structure to store the parameters of an incantation
typedef struct {
    int x;
    int y;
    int level;
    user_t *user;
    tile_t *tile;
    user_t *players[6];
    int player_count;
    char buffer[256];
} incantation_params_t;


typedef struct {
    int x;
    int y;
    int direction;
    int width;
    int height;
} position_t;

map_t *init_map(int width, int height);
void print_map(map_t *map);
void handle_gui_command(int, command_t *, const char *);
//function to handle the commands for the server to the gui
static void handle_msz(command_t *c_s);
static void handle_bct(command_t *c_s);
static void handle_tna(command_t *c_s);
static void handle_ppo(command_t *c_s);
static void handle_plv(command_t *c_s);
static void handle_pin(command_t *c_s);
void handle_sgt(command_t *c_s);
static void handle_sst(command_t *c_s);
void send_basique_gui_info(command_t *c_s);
//
tile_t *get_tile_from_coordinates(command_t *c_s, int x, int y);
void create_bct_response(command_t *c_s, tile_t *tile, int x, int y);
void handle_line_tiles(command_t *c_s, line_t *line, int y);
void handle_incantation(command_t *c_s, int sockfd, char *resp);
void eject(command_t *c_s, int sockfd, char *resp);
bool check_incantation_is_ready(command_t *c_s, int sockfd);
void distribute_resources(map_t *map, float count, const char *resource);
void regenerate_resources(map_t *map, command_t *c_s);
void handle_mct(command_t *c_s);
void create_tile_response(command_t *c_s, tile_t *tile, int x, int y);
void check_end_game(incantation_params_t *params, command_t *c_s, int i,
    char *);
int get_delta_y(int dy, int map_h);
int get_delta_x(int dx, int map_w);
int get_base_direction(int dx, int dy, int base_direction);
long calculate_elapsed_time(struct timeval *last_tick_time,
    struct timeval *current_time);
void handle_tick(command_t *c_s, struct timeval *last_tick_time,
    struct timeval *current_time, long elapsed_time);
void process_fds(command_t *c_s);
line_t *create_line(void);
tile_t *create_tile(void);
bool accept_connexion(command_t *c_s, int new_socket, char *buffer);
void initialize_server(command_t *c_s, struct timeval *last_tick_time);
