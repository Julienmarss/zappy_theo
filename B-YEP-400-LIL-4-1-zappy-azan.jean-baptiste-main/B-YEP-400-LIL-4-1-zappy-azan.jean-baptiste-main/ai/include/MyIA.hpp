#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <random>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>
#include <sstream>
#include <algorithm>


class MyIA {
public:
    int nb_food = 0;
    int nb_linemate = 0;
    std::string teamname = "";
    int nb_deraumere = 0;
    int nb_sibur = 0;
    int nb_mendiane = 0;
    int nb_phiras = 0;
    int nb_thystame = 0;
    int nb_global_food = 0;
    int nb_global_linemate = 0;
    int nb_command_send = 0;
    int nb_global_deraumere = 0;
    int nb_global_sibur = 0;
    int nb_global_mendiane = 0;
    int nb_global_phiras = 0;
    std::vector<std::string> steps;
    int nb_global_thystame = 0;
    std::string state = "FOOD_MOD";
    int level = 1;
    std::deque<std::string> reponse;
    int nb_player = 1;
    bool inventory_is_send = false;
    std::string client_num = "-1";
    int random_num;
    bool joined = false;
    bool is_chief = false;
    bool not_chief = false;
    bool chief_is_designed = false;
    bool incantation = false;
    bool meetup = false;
    int player_ready = 1;
    bool broadcast_is_sended = false;

    MyIA() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100000000);
        random_num = dis(gen);
    }

    void increment_food() { nb_food++; }
    void increment_linemate() { nb_linemate++; }
    void increment_deraumere() { nb_deraumere++; }
    void increment_sibur() { nb_sibur++; }
    void increment_mendiane() { nb_mendiane++; }
    void increment_phiras() { nb_phiras++; }
    void increment_thystame() { nb_thystame++; }
    void increment_level() { level++; }
    void decrement_food() { if (nb_food > 0) nb_food--; }
};
