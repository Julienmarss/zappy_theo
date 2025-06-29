/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** game.hpp
*/
#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include "gui_protocol.hpp"
#include "raylib_wrapper.hpp"
#include "Player.hpp"
#include "Team.hpp"
#include "Egg.hpp"
#include <random>
#include <deque>

class GuiProtocol;

struct BroadcastMessage {
    int playerId;
    std::string message;
    std::string teamName;
};

struct collectingMessage {
    int playerId;
    std::string message;
    std::string teamName;
};

struct GameMessage {
    int playerId;
    std::string teamName;
    std::string message;
    std::string messageType;
};

class Game {
public:
    enum GameState {
        MENU,
        GAME,
        END
    };
    Game(std::shared_ptr<RaylibWrapper> raylib) {};
    ~Game();
    void Run(std::shared_ptr<GuiProtocol> protocol);
    void DrawMap();
    void DrawTileInfo();
    void HandleMouseClick();
    void UpdatePlayerPosition(int id, const Vector3& position, int orientation);
    void SetPlayerIncantating(int id, bool isIncantating);
    Color GenerateRandomColor();
    void AddPlayer(const Player& player);
    std::shared_ptr<RaylibWrapper> raylib;
    std::vector<Player> getPlayers() { return playersInGame; }
    void UpdatePlayerInventory(int id, const std::array<int, 7>& inventory);
    std::vector<Player> getPlayerInGame() { return playersInGame; }
    std::deque<BroadcastMessage> broadcastMessages;
    std::deque<collectingMessage> collectingMessages;
    void CreateBroadCastMessage(int playerId, const std::string& team, const std::string& message);
    void CreatePlayerCollecting(int playerId, const std::string& team, const std::string& message);
    void DisplayBroadcastMessage(int playerId, const std::string& message, const std::string& teamName);
    void DisplayCollectingMessage(int playerId, const std::string& message, const std::string& teamName);
    void DrawMessages();
    void RemovePlayer(int id);
    void AddEgg(int eggId, int playerId, int x, int y);
    void EndGame(const std::string& winner);
    void UpdatePlayerLevel(int id, int level);
    void SetMapUtils(int timeUnit, int x, int y) {
        this->newTimeUnit = timeUnit;
        this->mapSizeW = x;
        this->mapSizeH = y;
    }
    void setGameState(GameState state) { this->gameState = state; }
private:
    Camera3D _camera;
    std::vector<Team> teams;
    std::vector<Player> playersPendingGraphics;
    std::vector<Player> playersInGame;
    std::vector<Egg> eggs;
    std::shared_ptr<GuiProtocol> _protocol;
    int animsCount;
    int animFrameCounter;
    bool animPlaying;
    float cubeSize = 1.0f;
    int timeUnit;
    int newTimeUnit = 0;
    int mapSizeW;
    int mapSizeH;
    bool inputActive;

    void LoadModelData();
    void UpdateModelAnimation();
    void DrawModel();
    void AddPlayerToTeam(const Player &player);
    void DrawTeams();
    void DrawMapUtils();
    void InitData();
    void InitPendingPlayers();
    void DrawPlayerInventory();
    void DrawResetCameraButton();
    void HandleInput();
    void DrawMenu();
    void EndGameVisuals();

    int selectedTileX = -1;
    int selectedTileY = -1;
    bool tileSelected = false;
    Ray ray;
    int selectedPlayerId = -1;
    RayCollision collision;
    std::unordered_map<int, std::array<int, 7>> playerInventory;
    std::vector<GameMessage> gameMessages;
    Texture2D background;
    Texture2D midground;
    Texture2D foreground;
    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;
    Music music;
    bool pause = false;
    GameState gameState = MENU;
    std::string winner;
};

#endif // GAME_HPP
