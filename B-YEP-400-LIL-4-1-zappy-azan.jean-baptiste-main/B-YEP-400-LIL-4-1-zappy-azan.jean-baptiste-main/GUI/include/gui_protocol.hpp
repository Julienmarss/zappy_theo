/*
** EPITECH PROJECT, 2024
** GAME
** File description:
** gui_protocol.hpp
*/

#ifndef GUI_PROTOCOL_HPP
#define GUI_PROTOCOL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Player.hpp"
#include "game.hpp"
#include <exception>

class Game;


struct MapTile {
    int x;
    int y;
    int q0;
    int q1;
    int q2;
    int q3;
    int q4;
    int q5;
    int q6;
    int resources[7];
};

struct PlayerPosition {
    int playerId;
    int x;
    int y;
    int orientation;
};

struct PlayerInventory {
    int playerId;
    int x;
    int y;
    int resources[7];
};

class GuiProtocol {
public:
    GuiProtocol(Game& gameInstance);
    void Connect(const std::string& host, int port);
    void Authenticate();
    void ProcessIncomingData();
    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }
    int getTimeUnit() const { return timeUnit; }
    std::unordered_map<int, MapTile> getMapTiles() const { return mapTiles; }
    std::unordered_map<int, MapTile> mapTiles;
    std::vector<Player> players;
    void assign_raylib(std::shared_ptr<RaylibWrapper> raylib) { _raylib = raylib; }
    std::vector<std::string> getTeamNames() const { return teamNames; }
    void RequestPlayerInventory(int playerId);
    class ProtocolException : public std::exception {
    public:
        ProtocolException(const std::string& message) : message(message) {}
        const char* what() const noexcept override { return message.c_str(); }
    private:
        std::string message;
    };

    void SendNewTimeUnit(int newTimeUnit);

private:
    int sockfd;
    int mapWidth;
    int mapHeight;
    int timeUnit;
    Game& gameInstance;
    std::vector<std::string> teamNames;
    std::shared_ptr<RaylibWrapper> _raylib;

    using CommandHandler = void (GuiProtocol::*)(const std::vector<std::string>&);
    std::unordered_map<std::string, CommandHandler> commandHandlers;

    void ParseMessage(const std::string& message);

    void handleMapSize(const std::vector<std::string>& parameters);
    void handleMapTile(const std::vector<std::string>& parameters);
    void handleMapContent(const std::vector<std::string>& parameters);
    void handleTeamName(const std::vector<std::string>& parameters);
    void handlePlayerPosition(const std::vector<std::string>& parameters);
    void handlePlayerLevel(const std::vector<std::string>& parameters);
    void handlePlayerInventory(const std::vector<std::string>& parameters);
    void handleTimeUnitRequest(const std::vector<std::string>& parameters);
    void NewPlayer(const std::vector<std::string>& parameters);
    void AuthDone(const std::vector<std::string>& parameters);
    void handleStartIncantation(const std::vector<std::string>& parameters);
    void handleEndIncantation(const std::vector<std::string>& parameters);
    void handleBroadcast(const std::vector<std::string>& parameters);
    void handleCollecting(const std::vector<std::string>& parameters);
    void DeathHandling(const std::vector<std::string>& parameters);
    void SucHandler(const std::vector<std::string>& parameters) { (void)parameters; return;}
    void NewEggHandler(const std::vector<std::string>& parameters);
    void EndGame(const std::vector<std::string>& parameters);
};

#endif // GUI_PROTOCOL_HPP
