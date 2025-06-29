#ifndef IA_HPP
#define IA_HPP

#include <vector>
#include <string>

class IA {
public:
    // Attributes from your Python code
    std::vector<std::string> steps;
    int nb_command_send;
    bool meetup;
    std::string state;
    int nb_global_linemate;
    int nb_global_deraumere;
    int nb_global_sibur;
    int nb_global_mendiane;
    int nb_global_phiras;
    int nb_global_thystame;
    
    int nb_linemate;
    int nb_deraumere;
    int nb_sibur;
    int nb_mendiane;
    int nb_phiras;
    int nb_thystame;
    
    std::string teamname;
    bool broadcast_is_sended;
    bool incantation;
    int player_ready;
    int nb_player;
    int level;
    int thystame;
    
    // Constructor
    IA() : nb_command_send(0), meetup(false), state(""), nb_global_linemate(0),
           nb_global_deraumere(0), nb_global_sibur(0), nb_global_mendiane(0),
           nb_global_phiras(0), nb_global_thystame(0), nb_linemate(0),
           nb_deraumere(0), nb_sibur(0), nb_mendiane(0), nb_phiras(0),
           nb_thystame(0), teamname(""), broadcast_is_sended(false),
           incantation(false), player_ready(0), nb_player(0), level(1),
           thystame(0) {}
    
    // Method to increment level (assuming this exists in your original code)
    void increment_level() {
        level++;
    }
};

#endif // IA_HPP