/*
** EPITECH PROJECT, 2024
** socket.h
** File description:
** Socket header file
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <uuid/uuid.h>
#include <sys/queue.h>
#include <errno.h>
#include "server.h"
#include <sys/time.h>
#include <math.h>

#define MAX_USERS c_s->clients_per_team * c_s->nb_teams

LIST_HEAD(team_user_head, user_t);
LIST_HEAD(teamshead, teams_t);
LIST_HEAD(linehead, line_t);
TAILQ_HEAD(queue_head, queue_t);
TAILQ_HEAD(egg_head, egg_t);

typedef struct map_s map_t;

typedef struct inventory_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
    int player;
} inventory_t;

typedef struct queue_t {
    char cmd[2048];
    int freq;
    TAILQ_ENTRY(queue_t) entries;
} queue_t;

typedef struct user_t {
    char uuid[37];
    int x;
    int y;
    int direction;
    int level;
    int id;
    char *team_name;
    int sockfd;
    inventory_t *inventory;
    LIST_ENTRY(user_t) entries;
    int nb_command;
    int loop_cmd;
    char *current_command;
    queue_t *queue;
    struct queue_head queueHead;
    struct egg_head eggHead;
    bool is_incantating;
} user_t;

//structure to store the egg parameters for the fork() command
typedef struct egg_t {
    int id;
    int x;
    int y;
    int hatch_time;
    TAILQ_ENTRY(egg_t) entries;
} egg_t;

typedef struct teams_t {
    user_t *user;
    struct team_user_head team_user_head;
    char uuid[37];
    int current_clients;
    int max_clients;
    char *team_name;
    int user_left;
    LIST_ENTRY(teams_t) entries;
} teams_t;

typedef struct command_t {
    struct sockaddr_in client_addr;
    struct teamshead teams_head;
    struct linehead line_head;
    socklen_t client_len;
    int data_socket;
    int nb_current_user;
    char buffer[1024];
    int fdmax;
    fd_set master_fds;
    fd_set read_fds;
    fd_set write_fds;
    teams_t *teams;
    struct sockaddr_in serv_addr;
    struct timeval timeout;
    int serv_sockfd;
    int port;
    int width;
    int height;
    int clients_per_team;
    int nb_teams;
    int freq;
    int argc;
    int finish;
    int unit_of_time;
    char **argv;
    map_t *map;
    int gui_socket;
    bool gui_connected;
    ///TIME
    struct timeval last_tick_time;
    struct timeval current_time;
    long elapsed_time;
    long remaining_time;
    int select_result;
} command_t;

typedef struct {
    user_t *user;
    int level;
    int offset;
    int x;
    int y;
} coordinates_t;


int config_socket(int port, command_t *c_s);

int start_server(command_t *c_s);

int parse_arguments(int argc, char **argv, command_t *c_s);

void signal_handler(int signal_number);

void setup_signal(void);

//////SET FUNCTION
int set_port(command_t *c_s, char *optarg);

int set_width(command_t *c_s, char *optarg);

int set_height(command_t *c_s, char *optarg);

int set_team_names(command_t *c_s, int argc, char **argv);

int set_clients_per_team(command_t *c_s, char *optarg);

void send_safe_response(int socket, char *response, command_t *c_s);

user_t *create_and_place_player(teams_t *, command_t *, int);

user_t *find_user_by_fd(command_t *c_s, int sockfd);

teams_t *find_team(command_t *c_s, char *team_name);

void enqueue_command(command_t *c_s, const char *command, int sockfd);

void tick_queue(command_t *c_s);

user_t *find_user_by_id(command_t *c_s, int id);

int get_object_id(char *resp);

///////////////////////COMMAND
void handle_comand(command_t *c_s, char *response, int sockfd);

int find_freq_command(command_t *, char *, int, queue_t *);

void mouvement_r(command_t *c_s, int sockfd, char *resp);

void mouvement_l(command_t *c_s, int sockfd, char *resp);

void mouvement_f(command_t *c_s, int sockfd, char *resp);

void vision(command_t *c_s, int sockfd, char *resp);

void broadcast(command_t *c_s, int sockfd, char *resp);

void inventory(command_t *c_s, int sockfd, char *resp);

void take_object(command_t *c_s, int sockfd, char *resp);

void mort(command_t *c_s, user_t *user);

void fork_p(command_t *c_s, int sockfd, char *resp);

char *remove_backslash_n(char *str);

void set_object(command_t *c_s, int sockfd, char *resp);

int config_socket(int port, command_t *c_s);

void init_server(command_t *c_s);

char *remove_backslash_n(char *str);

void handle_new_connection(command_t *c_s);

void handle_client_data(command_t *c_s, int i);
