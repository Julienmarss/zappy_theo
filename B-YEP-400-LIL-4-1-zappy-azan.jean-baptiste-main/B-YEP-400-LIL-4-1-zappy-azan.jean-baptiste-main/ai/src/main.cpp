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
#include "MyIA.hpp"

// Forward declarations
void handle_movement(int socket, MyIA& ia);
void take_action(MyIA& ia, const std::vector<std::string>& items);
void level_up(int socket, MyIA& ia);
void go_level_8(MyIA& ia);
void level_1_to_2(MyIA& ia);
void level_2_to_3(MyIA& ia);
void level_3_to_4(MyIA& ia);
void level_4_to_5(MyIA& ia);
void level_5_to_6(MyIA& ia);
void level_6_to_7(MyIA& ia);
void level_7_to_8(MyIA& ia);

int login(const std::string& ip, int port, const std::string& name, int timeout, MyIA& ia) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Erreur lors de la création du socket");
    }

    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        close(sock);
        throw std::runtime_error("Adresse invalide");
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        throw std::runtime_error("Erreur lors de la connexion au serveur");
    }

    char buffer[1024];
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Réponse : " << buffer << std::endl;
    }

    ia.teamname = name;
    std::string message = name + "\n";
    send(sock, message.c_str(), message.length(), 0);

    bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::string response(buffer);
        std::istringstream iss(response);
        std::string line;
        if (std::getline(iss, line)) {
            ia.client_num = line;
        }
        std::cout << "CLIENT: " << ia.client_num << std::endl;
        std::cout << "client num : " << response << std::endl;
    }

    return sock;
}

std::vector<std::string> get_steps_to_index(int index) {
    switch (index) {
        case 1: return {"Forward", "Left", "Forward"};
        case 2: return {"Forward"};
        case 3: return {"Forward", "Right", "Forward"};
        case 4: return {"Forward", "Forward", "Left", "Forward", "Forward"};
        case 5: return {"Forward", "Forward", "Left", "Forward"};
        case 6: return {"Forward", "Forward"};
        case 7: return {"Forward", "Forward", "Right", "Forward"};
        case 8: return {"Forward", "Forward", "Right", "Forward", "Forward"};
        default: return {};
    }
}

void choose_mov(int socket, MyIA& ia, const std::vector<std::vector<std::string>>& double_tableau) {
    int target_index = -1;
    int max_food_count = 0;
    int max_linemate_count = 0;
    int max_deraumere_count = 0;
    int max_sibur_count = 0;
    int max_mendiane_count = 0;
    int max_phiras_count = 0;
    int max_thystame_count = 0;

    for (size_t index = 0; index < double_tableau.size(); ++index) {
        const auto& tab = double_tableau[index];
        std::cout << "index: " << index << " tab: ";
        for (const auto& item : tab) {
            std::cout << item << " ";
        }
        std::cout << " mod : " << ia.state << std::endl;

        if (ia.level == 1 && std::find(tab.begin(), tab.end(), "linemate") != tab.end()) {
            target_index = index;
            break;
        } else if (std::find(tab.begin(), tab.end(), "food") != tab.end() && ia.state == "FOOD_MOD") {
            int food_count = std::count(tab.begin(), tab.end(), "food");
            if (food_count > max_food_count) {
                max_food_count = food_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "linemate") != tab.end() && ia.state == "LINEMATE_MOD") {
            int linemate_count = std::count(tab.begin(), tab.end(), "linemate");
            if (linemate_count > max_linemate_count) {
                max_linemate_count = linemate_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "deraumere") != tab.end() && ia.state == "DERAUMERE_MOD") {
            int deraumere_count = std::count(tab.begin(), tab.end(), "deraumere");
            if (deraumere_count > max_deraumere_count) {
                max_deraumere_count = deraumere_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "sibur") != tab.end() && ia.state == "SIBUR_MOD") {
            int sibur_count = std::count(tab.begin(), tab.end(), "sibur");
            if (sibur_count > max_sibur_count) {
                max_sibur_count = sibur_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "mendiane") != tab.end() && ia.state == "MENDIANE_MOD") {
            int mendiane_count = std::count(tab.begin(), tab.end(), "mendiane");
            if (mendiane_count > max_mendiane_count) {
                max_mendiane_count = mendiane_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "phiras") != tab.end() && ia.state == "PHIRAS_MOD") {
            int phiras_count = std::count(tab.begin(), tab.end(), "phiras");
            if (phiras_count > max_phiras_count) {
                max_phiras_count = phiras_count;
                target_index = index;
            }
        } else if (std::find(tab.begin(), tab.end(), "thystame") != tab.end() && ia.state == "THYSTAME_MOD") {
            int thystame_count = std::count(tab.begin(), tab.end(), "thystame");
            if (thystame_count > max_thystame_count) {
                max_thystame_count = thystame_count;
                target_index = index;
            }
        }
    }

    if (target_index != -1) {
        auto steps = get_steps_to_index(target_index);
        for (const auto& step : steps) {
            ia.steps.push_back(step);
        }
        handle_movement(socket, ia);
        take_action(ia, double_tableau[target_index]);
        level_up(socket, ia);
    } else {
        ia.steps.push_back("Forward");
        ia.steps.push_back("Forward");
        handle_movement(socket, ia);
    }
}

std::vector<std::vector<std::string>> look_and_parse(MyIA& ia) {
    std::string decoded_response = ia.reponse.front();
    ia.reponse.pop_front();
    std::cout << "Réponse brute: " << decoded_response << std::endl;
    
    // Remove brackets and split by comma
    std::string content = decoded_response.substr(1, decoded_response.length() - 2);
    std::istringstream iss(content);
    std::string case_str;
    std::vector<std::vector<std::string>> double_tableau;
    
    while (std::getline(iss, case_str, ',')) {
        // Trim whitespace
        case_str.erase(0, case_str.find_first_not_of(" \t"));
        case_str.erase(case_str.find_last_not_of(" \t") + 1);
        
        std::istringstream case_iss(case_str);
        std::string object;
        std::vector<std::string> objects;
        
        while (case_iss >> object) {
            objects.push_back(object);
        }
        
        if (objects.empty()) {
            objects.push_back("NONE");
        }
        
        double_tableau.push_back(objects);
    }
    
    std::cout << "Double tableau:" << std::endl;
    for (const auto& case_vec : double_tableau) {
        for (const auto& obj : case_vec) {
            std::cout << obj << " ";
        }
        std::cout << std::endl;
    }
    
    return double_tableau;
}

void get_and_update_inventory(MyIA& ia) {
    std::string inventory_after = ia.reponse.front();
    ia.reponse.pop_front();
    std::cout << "after inv: " << inventory_after << std::endl;
    
    // Remove brackets
    std::string content = inventory_after.substr(1, inventory_after.length() - 2);
    std::istringstream iss(content);
    std::string item;
    std::map<std::string, int> inventory_dict;
    
    while (std::getline(iss, item, ',')) {
        // Trim whitespace
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        
        std::istringstream item_iss(item);
        std::string name, count_str;
        item_iss >> name >> count_str;
        
        int count = 1;
        if (!count_str.empty() && std::all_of(count_str.begin(), count_str.end(), ::isdigit)) {
            count = std::stoi(count_str);
        }
        
        inventory_dict[name] = count;
    }
    
    // Update IA attributes
    ia.nb_food = inventory_dict["food"];
    ia.nb_linemate = inventory_dict["linemate"];
    ia.nb_deraumere = inventory_dict["deraumere"];
    ia.nb_sibur = inventory_dict["sibur"];
    ia.nb_mendiane = inventory_dict["mendiane"];
    ia.nb_phiras = inventory_dict["phiras"];
    ia.nb_thystame = inventory_dict["thystame"];
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void increment_global_inventory(MyIA& ia) {
    std::vector<std::string> rep = split(ia.reponse.front(), ',');
    std::vector<std::string> message = split(rep[1], '-');
    
    if (message[2] == "chief" && message[1] == ia.teamname) {
        ia.not_chief = true;
        ia.chief_is_designed = true;
        ia.reponse.pop_front();
        return;
    }
    
    if (message[2] == "jesuisla" && ia.state == "MEET_UP_CHIEF" && ia.teamname == message[1]) {
        ia.nb_player++;
        std::cout << "\n\n\n\n\nIL YYYYYYYYYYYY AAAAAAAAAAAAAAAAAA:    " << ia.nb_player << std::endl;
        std::cout << "\n\n\n\n\n\n" << std::endl;
        ia.reponse.pop_front();
        return;
    } else if (message[2] == "jesuisla" && ia.teamname == message[1] && ia.state != "MEET_UP_CHIEF") {
        ia.nb_player++;
        ia.reponse.pop_front();
        return;
    }
    
    if (message[2] == "je_suis_pret" && message[1] == ia.teamname && ia.state == "MEET_UP_CHIEF") {
        ia.player_ready++;
        ia.reponse.pop_front();
        std::cout << "\n\n\n\n JOUEUR PRET A INCANTER:   " << ia.player_ready << std::endl;
        std::cout << "\n\n\n\n" << std::endl;
        return;
    }
    
    if (message[2] == "je_suis_pret" && message[1] == ia.teamname && ia.state == "MEETUP") {
        ia.player_ready++;
        ia.reponse.pop_front();
        return;
    }
    
    if (message[1] == ia.teamname && message.size() >= 8) {
        ia.nb_global_linemate += std::stoi(message[2]);
        ia.nb_global_deraumere += std::stoi(message[3]);
        ia.nb_global_sibur += std::stoi(message[4]);
        ia.nb_global_mendiane += std::stoi(message[5]);
        ia.nb_global_phiras += std::stoi(message[6]);
        ia.nb_global_thystame += std::stoi(message[7]);
        
        std::cout << "\n global linemate : " << ia.nb_global_linemate << std::endl;
        std::cout << "\n global deraumere : " << ia.nb_global_deraumere << std::endl;
        std::cout << "\n global sibur : " << ia.nb_global_sibur << std::endl;
        std::cout << "\n global mendiane : " << ia.nb_global_mendiane << std::endl;
        std::cout << "\n global phiras : " << ia.nb_global_phiras << std::endl;
        std::cout << "\n global thystame : " << ia.nb_global_thystame << std::endl;
    }
    
    ia.reponse.pop_front();
}

void meetup(int socket, MyIA& ia) {
    std::vector<std::string> tab = split(ia.reponse.front(), ',');
    std::vector<std::string> tab2 = split(tab[0], ' ');
    std::string dir = tab2[1];
    std::cout << "je bouge : -" << dir << "-" << std::endl;
    
    std::vector<std::string> tab3 = split(tab[1], '_');
    int nb = std::stoi(tab3[3]);
    
    if (nb != ia.random_num) {
        return;
    }
    
    if (dir.find("0") != std::string::npos) {
        ia.steps.clear();
        if (!ia.joined) {
            std::string meetup_msg = "Broadcast -" + ia.teamname + "-jesuisla-";
            ia.steps.push_back(meetup_msg);
            ia.joined = true;
            ia.nb_player++;
        }
        std::cout << "\n\n\n ON EST ENSEMBLE \n\n\n" << std::endl;
        handle_movement(socket, ia);
        return;
    } else if (dir.find("3") != std::string::npos) {
        ia.steps.push_back("Left");
    } else if (dir.find("4") != std::string::npos) {
        ia.steps.push_back("Left");
    } else if (dir.find("5") != std::string::npos) {
        ia.steps.push_back("Left");
        ia.steps.push_back("Left");
    } else if (dir.find("6") != std::string::npos) {
        ia.steps.push_back("Right");
    } else if (dir.find("7") != std::string::npos) {
        ia.steps.push_back("Right");
    }
    
    ia.steps.push_back("Forward");
    std::string gooo = "Broadcast _" + ia.teamname + "_goo_" + std::to_string(ia.random_num);
    ia.steps.push_back(gooo);
    handle_movement(socket, ia);
}

// Level management functions are implemented in level_handling.cpp

void handle_movement(int socket, MyIA& ia) {
    while (ia.nb_command_send < 10 && !ia.steps.empty()) {
        std::string command = ia.steps[0] + "\n";
        send(socket, command.c_str(), command.length(), 0);
        ia.nb_command_send++;
        std::cout << "commande exec : " << command;
        ia.steps.erase(ia.steps.begin());
    }
}

void take_action(MyIA& ia, const std::vector<std::string>& items) {
    for (const auto& item : items) {
        if (item != "NONE" && item != "player") {
            std::string command = "Take " + item;
            ia.steps.push_back(command);
        }
    }
}

void level_up(int socket, MyIA& ia) {
    (void) socket; // Unused parameter, can be removed if not needed
    level_1_to_2(ia);
    level_2_to_3(ia);
    level_3_to_4(ia);
    level_4_to_5(ia);
    level_5_to_6(ia);
    level_6_to_7(ia);
    level_7_to_8(ia);
}

void main_game(int socket, MyIA& ia) {
    bool is_look = false;
    bool is_inventory = false;
    bool passeed = false;
    bool fork = true;
    
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket, &readfds);
        
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000; // 0.5 seconds
        
        int ready = select(socket + 1, &readfds, nullptr, nullptr, &timeout);
        
        if (ready > 0 && FD_ISSET(socket, &readfds)) {
            char buffer[1024];
            int bytes_received = recv(socket, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                std::string data(buffer);
                std::cout << "Message reçu: " << data << std::endl;
                
                std::vector<std::string> dbl_tab = split(data, '\n');
                for (const auto& elem : dbl_tab) {
                    if (!elem.empty()) {
                        ia.reponse.push_back(elem);
                    }
                }
                
                while (!ia.reponse.empty()) {
                    std::string response = ia.reponse.front();
                    
                    if (response.find("food") != std::string::npos && response.find("food") < 6) {
                        ia.nb_command_send--;
                        get_and_update_inventory(ia);
                        passeed = false;
                    } else if (response.find("player") != std::string::npos && response.find("player") < 8 && ia.state != "MEET_UP_CHIEF") {
                        ia.nb_command_send--;
                        auto double_tableau = look_and_parse(ia);
                        choose_mov(socket, ia, double_tableau);
                        passeed = true;
                    } else if (response.find("message") != std::string::npos && response.find("message") < 9) {
                        std::vector<std::string> rep = split(response, ',');
                        
                        if (rep.size() > 1 && rep[1].find("readymeetup") != std::string::npos && ia.state != "MEET_UP_CHIEF") {
                            std::vector<std::string> current_team = split(rep[1], '_');
                            if (current_team.size() > 1 && current_team[1] == ia.teamname) {
                                std::string gooo = "Broadcast _" + ia.teamname + "_goo_" + std::to_string(ia.random_num);
                                ia.steps.push_back(gooo);
                                ia.reponse.pop_front();
                                handle_movement(socket, ia);
                            } else {
                                ia.meetup = true;
                                ia.reponse.pop_front();
                            }
                        } else if (rep.size() > 1 && rep[1].find("readymeetup") != std::string::npos && ia.state == "MEET_UP_CHIEF") {
                            ia.reponse.pop_front();
                        }
                        
                        if (rep.size() > 1 && rep[1].find("_goo_") != std::string::npos && ia.state == "MEET_UP_CHIEF") {
                            if (ia.nb_player >= 6) {
                                ia.reponse.pop_front();
                                return;
                            }
                            std::vector<std::string> tab3 = split(rep[1], '_');
                            if (tab3.size() > 1 && tab3[1] == ia.teamname) {
                                std::string meetup_perso = "Broadcast _" + ia.teamname + "_meetup_" + tab3[3];
                                ia.steps.push_back(meetup_perso);
                                ia.reponse.pop_front();
                                handle_movement(socket, ia);
                            } else {
                                ia.reponse.pop_front();
                            }
                        } else if (rep.size() > 1 && rep[1].find("_goo_") != std::string::npos && ia.state != "MEET_UP_CHIEF") {
                            ia.reponse.pop_front();
                        }
                        
                        if (rep.size() > 1 && rep[1].find("_meetup_") != std::string::npos && ia.state != "MEET_UP_CHIEF") {
                            std::vector<std::string> mess = split(rep[1], '_');
                            if (mess.size() > 1 && mess[1] == ia.teamname) {
                                ia.state = "MEETUP";
                                meetup(socket, ia);
                                ia.reponse.pop_front();
                            } else {
                                ia.reponse.pop_front();
                            }
                        } else if (rep.size() > 1 && rep[1].find("_meetup_") != std::string::npos && ia.state == "MEET_UP_CHIEF") {
                            if (ia.nb_player >= 6) {
                                std::cout << "[MEETUP] Tous les joueurs sont là, début incantation" << std::endl;
                                ia.steps.push_back("Inventory");
                                ia.steps.push_back("Incantation");
                                ia.state = "LEVELING_UP";
                                ia.reponse.pop_front();
                                return;
                            }
                            ia.reponse.pop_front();
                        } else if (rep.size() > 1 && rep[1].find("-") != std::string::npos) {
                            increment_global_inventory(ia);
                        }
                    } else if ((response.find("ok") != std::string::npos && response.find("ok") < 4) ||
                               (response.find("ko") != std::string::npos && response.find("ko") < 4)) {
                        ia.nb_command_send--;
                        if (response.find("ko") != std::string::npos && response.find("ko") < 4 && ia.nb_player < 6) {
                            ia.steps.push_back("Left");
                        }
                        ia.reponse.pop_front();
                    } else {
                        ia.reponse.pop_front();
                    }
                }
            } else {
                std::cout << "Le serveur a fermé la connexion" << std::endl;
                close(socket);
                exit(1);
            }
        }
        
        if (ia.state != "MEETUP" && ia.state != "MEET_UP_CHIEF") {
            if (ia.nb_food < 20) {
                ia.state = "FOOD_MOD";
            }
            if (ia.nb_food > 70) {
                if (!ia.chief_is_designed) {
                    std::cout << "\n\n\n\n\n\n\n\nJE SUUUIS LE KIIIIIIIIIIIIIIIIIIIIIIIIING\n\n\n\n\n" << std::endl;
                    ia.chief_is_designed = true;
                    ia.is_chief = true;
                    ia.steps.clear();
                    std::string chief = "Broadcast -" + ia.teamname + "-chief";
                    ia.steps.push_back(chief);
                    handle_movement(socket, ia);
                }
                if (!ia.inventory_is_send) {
                    ia.inventory_is_send = true;
                    std::string inv = "Broadcast -" + ia.teamname + "-" + 
                                     std::to_string(ia.nb_linemate) + "-" +
                                     std::to_string(ia.nb_deraumere) + "-" +
                                     std::to_string(ia.nb_sibur) + "-" +
                                     std::to_string(ia.nb_mendiane) + "-" +
                                     std::to_string(ia.nb_phiras) + "-" +
                                     std::to_string(ia.nb_thystame);
                    ia.steps.push_back(inv);
                }
                if (ia.state == "FOOD_MOD") {
                    ia.state = "OUAIS";
                }
            }
            
            if (ia.state != "FOOD_MOD" && ia.state != "MEETUP") {
                if (fork) {
                    fork = false;
                    for (int i = 0; i < 5; i++) {
                        ia.steps.push_back("Fork");
                        handle_movement(socket, ia);
                    }
                }
                
                if (ia.nb_global_linemate < 8) {
                    ia.state = "LINEMATE_MOD";
                } else if (ia.nb_global_deraumere < 8) {
                    ia.state = "DERAUMERE_MOD";
                } else if (ia.nb_global_sibur < 10) {
                    ia.state = "SIBUR_MOD";
                } else if (ia.nb_global_mendiane < 5) {
                    ia.state = "MENDIANE_MOD";
                } else if (ia.nb_global_phiras < 6) {
                    ia.state = "PHIRAS_MOD";
                } else if (ia.nb_global_thystame < 1) {
                    ia.state = "THYSTAME_MOD";
                } else if (ia.state != "MEETUP") {
                    if (ia.is_chief && !ia.not_chief) {
                        ia.state = "MEET_UP_CHIEF";
                        std::string phrase = "Broadcast _" + ia.teamname + "_readymeetup";
                        ia.steps.push_back(phrase);
                    }
                }
            }
            
            if (!passeed && !is_look && ia.state != "MEETUP" && ia.state != "MEET_UP_CHIEF") {
                ia.steps.push_back("Look");
                is_look = true;
                is_inventory = false;
                handle_movement(socket, ia);
            } else if (passeed && !is_inventory && ia.state != "MEETUP" && ia.state != "MEET_UP_CHIEF") {
                ia.steps.push_back("Inventory");
                handle_movement(socket, ia);
                is_look = false;
                is_inventory = true;
            }
        }
        
        if (ia.nb_player >= 6) {
            go_level_8(ia);
        }
        
        handle_movement(socket, ia);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 7) {
        std::cout << "Usage: ./zappy_ai -p <port> -n <name> -h <host>" << std::endl;
        return 1;
    }

    std::string host;
    int port = -1;
    std::string name;
    bool has_h = false;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-h") {
            has_h = true;
            if (i + 1 < argc) {
                host = argv[i + 1];
                i++;
            }
        } else if (std::string(argv[i]) == "-p") {
            if (i + 1 < argc) {
                port = std::stoi(argv[i + 1]);
                i++;
            }
        } else if (std::string(argv[i]) == "-n") {
            if (i + 1 < argc) {
                name = argv[i + 1];
                i++;
            }
        }
    }

    if (!has_h || port == -1 || name.empty() || host.empty()) {
        std::cout << "Missing required arguments: -p <port>, -n <name>, -h <host>" << std::endl;
        return 1;
    }

    MyIA my_ia;
    int socket_fd;
    
    try {
        socket_fd = login(host, port, name, 5, my_ia);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    if (socket_fd > 0) {
        try {
            main_game(socket_fd, my_ia);
        } catch (const std::exception& e) {
            std::cout << "Erreur dans le jeu: " << e.what() << std::endl;
            close(socket_fd);
            return 1;
        }
    } else {
        return 1;
    }

    close(socket_fd);
    return 0;
}