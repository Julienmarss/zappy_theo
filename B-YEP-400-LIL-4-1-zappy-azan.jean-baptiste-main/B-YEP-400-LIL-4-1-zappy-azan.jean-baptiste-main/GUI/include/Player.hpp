/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Player.hpp
*/

#ifndef ZAPPY_PLAYER_HPP
#define ZAPPY_PLAYER_HPP

#include "IObjet.hpp"

class RaylibWrapper;

class Player : public IObject {
public:
    Player(int id, Vector3 position, int orientation, int level, std::string teamName, std::shared_ptr<RaylibWrapper> raylibWrapper);
    ~Player() = default;

    int getId() const override { return id; }
    int getX() const override { return static_cast<int>(position.x); }
    int getY() const override { return static_cast<int>(position.z); }
    void setPosition(int x, int y) override { position.x = x; position.z = y; }

    void Draw() override;
    std::string getTeamName() const { return teamName; }
    void initPlayerGraphics();
    void SetPosition(Vector3 newPosition, int orientation);
    void UpdateAnimation();
    void SetColor(Color color);
    void UpdateInventory(const std::array<int, 7>& newInventory);
    void SetIncantating(bool isIncantating) {this->isIncantating = isIncantating;}
    int id;
    Vector3 position;
    int orientation;
    std::string teamName;

    class PlayerException : public std::exception {
    public:
        PlayerException(const std::string &message) : _message(message) {}
        const char *what() const noexcept override { return _message.c_str(); }
    private:
        std::string _message;
    };
    int level;

private:
    Color playerColor;
    std::shared_ptr<RaylibWrapper> raylib;
    Model playerModel;
    bool modelLoaded;
    const char* modelPath = "GUI/assets/3d/cesium_man.m3d";
    int animationCount = 0;
    int animFrameCounter = 0;
    ModelAnimation* animations = nullptr;
    bool isWalking = false;
    std::array<int, 7> inventory;
    bool isIncantating;
};

#endif //ZAPPY_PLAYER_HPP
