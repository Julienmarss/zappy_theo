#include "take_object.hpp"
#include "mouv.hpp"
#include "IA.hpp"
#include <iostream>
#include <algorithm>
#include "MyIA.hpp"
void take_action(int s, MyIA& ia, const std::vector<std::string>& tab) {
    if (ia.meetup == true && std::find(tab.begin(), tab.end(), "player") != tab.end()) {
        return;
    }
    for (const auto& element : tab) {
        if (element == "food") {
            take_food(ia);
        }
        if ((element == "linemate" && ia.state == "FOOD_MOD") || 
            (element == "linemate" && ia.state == "LINEMATE_MOD" && ia.nb_global_linemate < 8)) {
            take_linemate(ia);
        }
        if ((element == "deraumere" && ia.state == "FOOD_MOD") || 
            (element == "deraumere" && ia.state == "DERAUMERE_MOD" && ia.nb_global_deraumere < 8)) {
            take_deraumere(ia);
        }
        if ((element == "sibur" && ia.state == "FOOD_MOD") || 
            (element == "sibur" && ia.state == "SIBUR_MOD" && ia.nb_global_sibur < 10)) {
            take_sibur(ia);
        }
        if ((element == "mendiane" && ia.state == "FOOD_MOD") || 
            (element == "mendiane" && ia.state == "MENDIANE_MOD" && ia.nb_global_mendiane < 5)) {
            take_mendiane(ia);
        }
        if ((element == "phiras" && ia.state == "FOOD_MOD") || 
            (element == "phiras" && ia.state == "PHIRAS_MOD" && ia.nb_global_phiras < 6)) {
            take_phiras(ia);
        }
        if ((element == "thystame" && ia.state == "FOOD_MOD") || 
            (element == "thystame" && ia.state == "THYSTAME_MOD" && ia.nb_global_thystame < 1)) {
            take_thystame(ia);
        }
        handle_movement(s, ia);
    }
}

void take_food(MyIA& ia) {
    ia.steps.push_back("Take food");
    std::cout << "reponse take food:" << std::endl;
}

void take_linemate(MyIA& ia) {
    ia.steps.push_back("Take linemate");
    std::cout << "reponse take linemate:" << std::endl;
    ia.nb_global_linemate++;
}

void take_deraumere(MyIA& ia) {
    ia.steps.push_back("Take deraumere");
    std::cout << "reponse take deraumere:" << std::endl;
    ia.nb_global_deraumere++;
}

void take_sibur(MyIA& ia) {
    ia.steps.push_back("Take sibur");
    std::cout << "reponse take sibur:" << std::endl;
    ia.nb_global_sibur++;
}

void take_mendiane(MyIA& ia) {
    ia.steps.push_back("Take mendiane");
    std::cout << "reponse take mendiane:" << std::endl;
    ia.nb_global_mendiane++;
}

void take_phiras(MyIA& ia) {
    ia.steps.push_back("Take phiras");
    std::cout << "reponse take phiras:" << std::endl;
    ia.nb_global_phiras++;
}

void take_thystame(MyIA& ia) {
    ia.steps.push_back("Take thystame");
    std::cout << "reponse take thystame:" << std::endl;
    ia.nb_global_thystame++;
}