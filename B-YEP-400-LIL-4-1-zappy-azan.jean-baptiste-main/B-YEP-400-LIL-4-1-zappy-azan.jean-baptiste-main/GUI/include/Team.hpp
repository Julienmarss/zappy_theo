//
// Created by Jean-Baptiste  Azan on 13/06/2024.
//

#ifndef ZAPPY_TEAM_HPP
#define ZAPPY_TEAM_HPP
#include <string>
#include "Player.hpp"
#include <vector>

class Team {
public:
    std::string name;
    std::vector<Player> players;

    Team(const std::string &teamName) : name(teamName) {}

    void AddPlayer(const Player &player);
//    void DrawPlayers();
    std::vector<Player>& GetPlayers() {
        return players;
    };
};


#endif //ZAPPY_TEAM_HPP
