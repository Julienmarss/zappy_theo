/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Egg.cpp
*/

#include "Egg.hpp"

Egg::Egg(int id, int x, int y, int playerId)
        : _id(id), _x(x), _y(y), _playerId(playerId), _isHatching(false) {}


void Egg::Draw() {
    int radiusY = 30;
    Vector3 position = {static_cast<float>(_x), 0.5f, static_cast<float>(_y)};

    raylib->DrawSphere(position, 0.5, BLUE);
}
