#include "mouv.hpp"
#include "IA.hpp"
#include <iostream>
#include <string>
#include "MyIA.hpp"

void handle_movement(int s, IA& ia) {
    if (ia.nb_command_send < 10 && !ia.steps.empty()) {
        std::string command = ia.steps[0] + "\n";
        send(s, command.c_str(), command.length(), 0);
        ia.nb_command_send++;
        std::cout << "commande exec : " << command;
        ia.steps.erase(ia.steps.begin());
        handle_movement(s, ia);
    }
}