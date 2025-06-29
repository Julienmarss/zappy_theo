#include "level_handling.hpp"
#include "mouv.hpp"
#include "IA.hpp"
#include <iostream>
#include <string>
#include "MyIA.hpp"

void go_level_8(MyIA& ia) {
    while (ia.nb_linemate > 0) {
        ia.steps.push_back("Set linemate");
        ia.nb_linemate--;
    }
    while (ia.nb_deraumere > 0) {
        ia.steps.push_back("Set deraumere");
        ia.nb_deraumere--;
    }
    while (ia.nb_sibur > 0) {
        ia.steps.push_back("Set sibur");
        ia.nb_sibur--;
    }
    while (ia.nb_mendiane > 0) {
        ia.steps.push_back("Set mendiane");
        ia.nb_mendiane--;
    }
    while (ia.nb_phiras > 0) {
        ia.steps.push_back("Set phiras");
        ia.nb_phiras--;
    }
    while (ia.nb_thystame > 0) {
        ia.steps.push_back("Set thystame");
        ia.nb_thystame--;
    }
    if (ia.state != "MEET_UP_CHIEF" && ia.broadcast_is_sended == false) {
        ia.broadcast_is_sended = true;
        std::string broadcast = "Broadcast -" + ia.teamname + "-je_suis_pret-";
        ia.steps.push_back(broadcast);
    }
    if (ia.state == "MEET_UP_CHIEF" && ia.incantation == false && ia.player_ready >= 6) {
        ia.incantation = true;
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
        ia.steps.push_back("Incantation");
    }
}

void level_1_to_2(MyIA& ia) {
    if (ia.nb_linemate >= 1 && ia.level == 1) {
        std::cout << "\nlevel up\n" << std::endl;
        ia.steps.push_back("Set linemate");
        ia.steps.push_back("Incantation");
        ia.increment_level();
        ia.nb_linemate--;
        ia.nb_global_linemate--;
    }
}

void level_2_to_3(MyIA& ia) {
    if (ia.nb_player >= 2 && ia.level == 2 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 1 && ia.nb_global_sibur == 1) {
        std::cout << "\nlevel up\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

void level_3_to_4(MyIA& ia) {
    if (ia.nb_player >= 2 && ia.level == 3 && ia.nb_global_linemate == 2 && 
        ia.nb_global_sibur == 1 && ia.nb_global_phiras == 2) {
        std::cout << "\nlevel up\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

void level_4_to_5(MyIA& ia) {
    if (ia.nb_player >= 4 && ia.level == 4 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 1 && ia.nb_global_sibur == 2 && ia.nb_global_phiras == 1) {
        std::cout << "\nlevel up\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

void level_5_to_6(MyIA& ia) {
    if (ia.nb_player >= 4 && ia.level == 5 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 2 && ia.nb_global_sibur == 1 && ia.nb_global_mendiane == 3) {
        std::cout << "\nlevel up\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_mendiane > 0) {
            ia.steps.push_back("Set mendiane");
            ia.nb_mendiane--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

void level_6_to_7(MyIA& ia) {
    if (ia.nb_player >= 6 && ia.level == 6 && ia.nb_global_linemate == 1 && 
        ia.nb_global_deraumere == 2 && ia.nb_global_sibur == 3 && ia.nb_global_phiras == 1) {
        std::cout << "\nlevel up\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}

void level_7_to_8(MyIA& ia) {
    if (ia.nb_player >= 6 && ia.level == 7 && ia.nb_global_linemate == 2 && 
        ia.nb_global_deraumere == 2 && ia.nb_global_sibur == 2 && ia.nb_global_mendiane == 2 && 
        ia.nb_global_phiras == 2 && ia.nb_thystame == 1) {
        std::cout << "\nlevel up\n" << std::endl;
        while (ia.nb_linemate > 0) {
            ia.steps.push_back("Set linemate");
            ia.nb_linemate--;
        }
        while (ia.nb_deraumere > 0) {
            ia.steps.push_back("Set deraumere");
            ia.nb_deraumere--;
        }
        while (ia.nb_sibur > 0) {
            ia.steps.push_back("Set sibur");
            ia.nb_sibur--;
        }
        while (ia.nb_mendiane > 0) {
            ia.steps.push_back("Set mendiane");
            ia.nb_mendiane--;
        }
        while (ia.nb_phiras > 0) {
            ia.steps.push_back("Set phiras");
            ia.nb_phiras--;
        }
        while (ia.nb_thystame > 0) {
            ia.steps.push_back("Set thystame");
            ia.nb_thystame--;
        }
        ia.steps.push_back("Incantation");
        ia.level++;
    }
}