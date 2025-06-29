/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Egg.hpp
*/

#pragma once

#include "IObjet.hpp"

class Egg : public IObject {
public:
    Egg(int id, int x, int y, int playerId);
    ~Egg() = default;

    int getId() const override { return _id; }
    int getX() const override { return _x; }
    int getY() const override { return _y; }
    void setPosition(int x, int y) override { _x = x; _y = y; }
    void Draw() override;

private:
    int _id;
    int _x;
    int _y;
    int _playerId;
    bool _isHatching;
    std::shared_ptr<RaylibWrapper> raylib;
};
