//
// Created by Jean-Baptiste  Azan on 13/06/2024.
//

#include "Player.hpp"

Player::Player(int id, Vector3 position, int orientation, int level, std::string teamName, std::shared_ptr<RaylibWrapper> raylib) {
    this->id = id;
    this->position = position;
    this->orientation = orientation;
    this->level = level;
    this->teamName = teamName;
    this->raylib = raylib;
    this->modelLoaded = false;
    this->isIncantating = false;
}

void Player::SetPosition(Vector3 newPosition, int orientation) {
    if (this->position.x != newPosition.x || this->position.z != newPosition.z)
        this->isWalking = true;
    else
        this->isWalking = false;
    this->position = newPosition;
    this->orientation = orientation;
}


void Player::initPlayerGraphics() {
    if (modelLoaded)
        return;
    playerModel = raylib->LoadModel(modelPath);
    if (playerModel.meshCount == 0) {
        throw Player::PlayerException("Failed to load model");
    } else {
        modelLoaded = true;
        animations = raylib->LoadModelAnimations(modelPath, &animationCount);
        if (animationCount == 0)
            throw Player::PlayerException("Failed to load animations");
    }
}

void Player::UpdateAnimation() {
    if (isWalking && animationCount > 0) {
        animFrameCounter++;
        if (animFrameCounter >= animations[0].frameCount) animFrameCounter = 0;
        raylib->UpdateModelAnimation(playerModel, animations[0], animFrameCounter);
    }
}

void Player::SetColor(Color color) {
    playerColor = color;
}

void Player::UpdateInventory(const std::array<int, 7>& newInventory) {
    inventory = newInventory;
}

void Player::Draw() {
    if (!modelLoaded) {
        throw Player::PlayerException("Model not loaded");
        return;
    }
    UpdateAnimation();
    Matrix rotation;
    switch (orientation) {
        case 1:
            rotation = MatrixRotateXYZ((Vector3){0.0f, 0.0f, 0.0f});
            break;
        case 2:
            rotation = MatrixRotateXYZ((Vector3){0.0f, PI / 2, 0.0f});
            break;
        case 3:
            rotation = MatrixRotateXYZ((Vector3){0.0f, PI, 0.0f});
            break;
        case 4:
            rotation = MatrixRotateXYZ((Vector3){0.0f, -PI / 2, 0.0f});
            break;
        default:
            rotation = MatrixIdentity();
            break;
    }
    playerModel.transform = rotation;
    raylib->DrawModel(playerModel, position, 1.0f, playerColor);
    if (isIncantating)
        raylib->DrawModelWires(playerModel, position, 1.0f, RED);
}

