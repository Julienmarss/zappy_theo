#include "../include/game.hpp"
#include "colors.hpp"

Game::~Game() {
    raylib->UnloadTexture(background);
    raylib->UnloadTexture(midground);
    raylib->UnloadTexture(foreground);
    raylib->UnloadMusicStream(music);
    gameMessages.clear();
    playersInGame.clear();
    teams.clear();
    playerInventory.clear();
    broadcastMessages.clear();
    collectingMessages.clear();
}

Color Game::GenerateRandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return Color{static_cast<unsigned char>(dis(gen)), static_cast<unsigned char>(dis(gen)), static_cast<unsigned char>(dis(gen)), 255};
}

void Game::RemovePlayer(int id) {
    for (auto it = playersInGame.begin(); it != playersInGame.end(); ++it) {
        if (it->id == id) {
            playersInGame.erase(it);
            break;
        }
    }
}

void Game::AddEgg(int eggId, int playerId, int x, int y) {
    Egg egg(eggId, x, y, playerId);
    eggs.push_back(egg);
}

void Game::AddPlayer(const Player& player) {
    playersPendingGraphics.push_back(player);
}

void Game::UpdatePlayerInventory(int id, const std::array<int, 7>& inventory) {
    playerInventory[id] = inventory;
}

void Game::UpdatePlayerLevel(int id, int level) {
    for (auto& player : playersInGame) {
        if (player.id == id) {
            player.level = level;
            break;
        }
    }
}

void Game::CreateBroadCastMessage(int playerId, const std::string& message, const std::string& teamName) {
    BroadcastMessage broadcastMessage;
    broadcastMessage.playerId = playerId;
    broadcastMessage.message = message;
    broadcastMessage.teamName = teamName;
    broadcastMessages.push_back(broadcastMessage);
}

void Game::CreatePlayerCollecting(int playerId, const std::string& message, const std::string& teamName) {
    BroadcastMessage collectMessage;
    collectMessage.playerId = playerId;
    collectMessage.message = message;
    collectMessage.teamName = teamName;
    broadcastMessages.push_back(collectMessage);
}

void Game::DisplayBroadcastMessage(int playerId, const std::string& message, const std::string& teamName) {
    GameMessage gameMessage = { playerId, teamName, message, "BROADCAST" };
    gameMessages.push_back(gameMessage);
}

void Game::DisplayCollectingMessage(int playerId, const std::string& message, const std::string& teamName) {
    GameMessage gameMessage = { playerId, teamName, message, "COLLECTING" };
    gameMessages.push_back(gameMessage);
}

void Game::DrawMessages() {
    int posY = GetScreenHeight() - 20 * (gameMessages.size() + 1);
    if (gameMessages.size() > 20)
        gameMessages.erase(gameMessages.begin());
    for (const auto& msg : gameMessages) {
        std::string displayMessage = "[" + msg.messageType + " - " + msg.teamName + " : " + std::to_string(msg.playerId) + "]: " + msg.message;
        raylib->DrawText(displayMessage.c_str(), 10, posY, 20, BLACK);
        raylib->DrawRectangle(10, posY, 600, 20, Fade(RED, 0.7f));
        posY += 20;
    }
}

void Game::SetPlayerIncantating(int id, bool isIncantating) {
    for (auto& player : playersInGame) {
        if (player.id == id) {
            player.SetIncantating(isIncantating);
            break;
        }
    }
}

void Game::UpdatePlayerPosition(int id, const Vector3& position, int orientation) {
    for (auto& player : playersInGame) {
        if (player.id == id) {
            player.SetPosition(position, orientation);
            break;
        }
    }
}

void Game::InitPendingPlayers() {
    for (auto& player : playersPendingGraphics) {
        player.SetPosition(player.position, 1);
        player.initPlayerGraphics();
        AddPlayerToTeam(player);
        playersInGame.push_back(player);
    }
    playersPendingGraphics.clear();
}

void Game::AddPlayerToTeam(const Player& player) {
    for (auto& team : teams) {
        if (team.name == player.teamName) {
            team.AddPlayer(player);
            return;
        }
    }
    Team newTeam(player.teamName);
    newTeam.AddPlayer(player);
    teams.push_back(newTeam);
}

void Game::DrawMenu() {
    scrollingBack -= 0.1f;
    scrollingMid -= 0.5f;
    scrollingFore -= 1.0f;
    if (scrollingBack <= -background.width * 2)
        scrollingBack = 0;
    if (scrollingMid <= -midground.width * 2)
        scrollingMid = 0;
    if (scrollingFore <= -foreground.width * 2)
        scrollingFore = 0;
    raylib->BeginDrawing();
    raylib->ClearBackground(DARKPURPLE);
    raylib->DrawTextureEx(background, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(background, (Vector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(midground, (Vector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(foreground, (Vector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
    raylib->DrawText("ZAPPY", 20, 30, 20, RED);
    if (raylib->CheckCollisionPointRec(raylib->GetMousePosition(), (Rectangle){ 400, 700, 200, 30 })) {
        if (raylib->IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            gameState = GAME;
        raylib->DrawRectangle(410, 700, 200, 30, GRAY);
    } else
        raylib->DrawRectangle(410, 700, 200, 30, LIGHTGRAY);
    raylib->DrawText("Start Game", 450, 710, 20, BLACK);
    raylib->EndDrawing();
}

void Game::EndGameVisuals()
{
    scrollingBack -= 0.1f;
    scrollingMid -= 0.5f;
    scrollingFore -= 1.0f;
    if (scrollingBack <= -background.width * 2)
        scrollingBack = 0;
    if (scrollingMid <= -midground.width * 2)
        scrollingMid = 0;
    if (scrollingFore <= -foreground.width * 2)
        scrollingFore = 0;
    raylib->BeginDrawing();
    raylib->ClearBackground(DARKPURPLE);
    raylib->DrawTextureEx(background, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(background, (Vector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(midground, (Vector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
    raylib->DrawTextureEx(foreground, (Vector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
    raylib->DrawText("ZAPPY", 20, 30, 20, RED);
    raylib->DrawText(("Game finished : Team " + winner + " won").c_str(), 350, 700, 20, WHITE);
    raylib->EndDrawing();
}

void Game::EndGame(const std::string &winner) {
    this->winner = winner;
    gameState = END;
}

void Game::Run(std::shared_ptr<GuiProtocol> protocol) {
    _protocol = protocol;

    InitData();
    _camera = { 0 };
    _camera.position = (Vector3){ 20.0f, 20.0f, 20.0f };
    _camera.target = (Vector3){ 5.0f, 0.0f, 5.0f };
    _camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;
    raylib->SetTargetFPS(60);
    while (!raylib->WindowShouldClose()) {
        UpdateMusicStream(this->music);
        PlayMusicStream(this->music);
        if (gameState == MENU)
            DrawMenu();
        else if (gameState == GAME) {
            raylib->UpdateCamera(&_camera, CAMERA_FREE);
            HandleMouseClick();
            InitPendingPlayers();
            raylib->BeginDrawing();
            raylib->ClearBackground(DARKPURPLE);
            raylib->BeginMode3D(_camera);
            DrawMap();
            DrawTeams();
            raylib->EndMode3D();
            raylib->EndDrawing();
            HandleInput();
            DrawTileInfo();
            DrawMessages();
            DrawMapUtils();
            DrawPlayerInventory();
        } else if (gameState == END)
             EndGameVisuals();
    }
}

void Game::DrawTeams() {
    for(auto& player : playersInGame) {
        player.Draw();
    }
    for (auto& egg : eggs) {
        egg.Draw();
    }
}

void Game::DrawMap() {
    const float pointSize = 0.2f;
    const float offset = 0.3f;
    int mapWidth = _protocol->getMapWidth();
    int mapHeight = _protocol->getMapHeight();
    Vector3 offsets[9] = {
        {-offset, 0, -offset}, {0, 0, -offset}, {offset, 0, -offset},
        {-offset, 0, 0},       {0, 0, 0},       {offset, 0, 0},
        {-offset, 0, offset},  {0, 0, offset},  {offset, 0, offset}
    };

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            Vector3 cubePosition = { x * cubeSize, 0.0f, y * (cubeSize) };
            Color cubeColor = BLACK;
            if (tileSelected && x == selectedTileX && y == selectedTileY) {
                cubeColor = MAROON;
            }
            raylib->DrawCube(cubePosition, cubeSize, cubeSize, cubeSize, cubeColor);
            raylib->DrawCubeWires(cubePosition, cubeSize, cubeSize, cubeSize, WHITE);
            int tileKey = x + y * mapWidth;
            if (_protocol->mapTiles.find(tileKey) != _protocol->mapTiles.end()) {
                MapTile& tile = _protocol->mapTiles.at(tileKey);
                for (int i = 0; i < 7; ++i) {
                    if (tile.resources[i] > 0) {
                        Color color;
                        switch (i) {
                            case 0: color = FOOD_COLOR; break;
                            case 1: color = LINEMATE_COLOR; break;
                            case 2: color = DERAUMERE_COLOR; break;
                            case 3: color = SIBUR_COLOR; break;
                            case 4: color = MENDIANE_COLOR; break;
                            case 5: color = PHIRAS_COLOR; break;
                            case 6: color = THYSTAME_COLOR; break;
                            default: color = WHITE; break;
                        }
                        Vector3 pointPosition = {
                            cubePosition.x + offsets[i].x,
                            cubePosition.y + cubeSize / 2,
                            cubePosition.z + offsets[i].z
                        };
                        raylib->DrawCube(pointPosition, pointSize, pointSize, pointSize, color);
                    }
                }
            }
        }
    }
}

void Game::DrawTileInfo() {
    if (!tileSelected) {
        return;
    }
    int tileKey = selectedTileX + selectedTileY * _protocol->getMapWidth();
    if (_protocol->mapTiles.find(tileKey) != _protocol->mapTiles.end()) {
        MapTile& tile = _protocol->mapTiles.at(tileKey);
        std::string info = "Tile (" + std::to_string(selectedTileX) + ", " + std::to_string(selectedTileY) + ")\n";
        info += "Food: " + std::to_string(tile.resources[0]) + "\n";
        info += "Linemate: " + std::to_string(tile.resources[1]) + "\n";
        info += "Deraumere: " + std::to_string(tile.resources[2]) + "\n";
        info += "Sibur: " + std::to_string(tile.resources[3]) + "\n";
        info += "Mendiane: " + std::to_string(tile.resources[4]) + "\n";
        info += "Phiras: " + std::to_string(tile.resources[5]) + "\n";
        info += "Thystame: " + std::to_string(tile.resources[6]) + "\n";
        raylib->DrawRectangle(10, 10, 200, 180, Fade(LIGHTGRAY, 0.7f));
        raylib->DrawText(info.c_str(), 20, 20, 20, BLACK);
    }
}

void Game::HandleMouseClick() {
    int mapWidth = _protocol->getMapWidth();
    int mapHeight = _protocol->getMapHeight();
    if (!raylib->IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }
    Ray ray = raylib->GetMouseRay(raylib->GetMousePosition(), _camera);
    RayCollision collision = {0};
    const float cubeSize = 1.0f;
    for (auto &player: playersInGame) {
        BoundingBox box = {
            {player.position.x - cubeSize / 2, player.position.y - cubeSize / 2, player.position.z - cubeSize / 2},
            {player.position.x + cubeSize / 2, player.position.y + cubeSize / 2, player.position.z + cubeSize / 2}
        };
        collision = raylib->GetRayCollisionBox(ray, box);
        if (collision.hit) {
            selectedPlayerId = player.id;
            _protocol->RequestPlayerInventory(player.id);
            return;
        }
    }
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            Vector3 cubePosition = {x * (cubeSize), 0.0f, y * (cubeSize)};
            BoundingBox box = {
                {cubePosition.x - cubeSize / 2, cubePosition.y - cubeSize / 2, cubePosition.z - cubeSize / 2},
                {cubePosition.x + cubeSize / 2, cubePosition.y + cubeSize / 2, cubePosition.z + cubeSize / 2}
            };
            collision = raylib->GetRayCollisionBox(ray, box);
            if (collision.hit) {
                selectedTileX = x;
                selectedTileY = y;
                tileSelected = true;
                return;
            }
        }
    }
}

void Game::InitData() {
    const auto& teamNames = _protocol->getTeamNames();
    for (const auto& teamName : teamNames) {
        teams.emplace_back(teamName);
    }
    this->background = raylib->LoadTexture("GUI/assets/cyberpunk_street_background.png");
    this->midground = raylib->LoadTexture("GUI/assets/cyberpunk_street_midground.png");
    this->foreground = raylib->LoadTexture("GUI/assets/cyberpunk_street_foreground.png");
    raylib->InitAudioDevice();
    this->music = raylib->LoadMusicStream("GUI/assets/audio/bg_music.mp3");
}

void Game::DrawPlayerInventory() {
    if (selectedPlayerId != -1 && playerInventory.find(selectedPlayerId) != playerInventory.end()) {
        const auto& inventory = playerInventory[selectedPlayerId];
        std::string info = "Player Inventory:\n";
        info += "Food: " + std::to_string(inventory[0]) + "\n";
        info += "Linemate: " + std::to_string(inventory[1]) + "\n";
        info += "Deraumere: " + std::to_string(inventory[2]) + "\n";
        info += "Sibur: " + std::to_string(inventory[3]) + "\n";
        info += "Mendiane: " + std::to_string(inventory[4]) + "\n";
        info += "Phiras: " + std::to_string(inventory[5]) + "\n";
        info += "Thystame: " + std::to_string(inventory[6]) + "\n";
        for (auto& player : playersInGame) {
            if (player.id == selectedPlayerId) {
                info += "Player Level: " + std::to_string(player.level) + "\n";
                break;
            }
        }
        raylib->DrawRectangle(10, 200, 200, 180, Fade(LIGHTGRAY, 0.7f));
        raylib->DrawText(info.c_str(), 20, 210, 20, WHITE);
    }
}

void Game::HandleInput() {
    if (IsKeyPressed(KEY_O)) {
        newTimeUnit++;
        _protocol->SendNewTimeUnit(newTimeUnit);
    }
    if (IsKeyPressed(KEY_L)) {
        if (newTimeUnit > 1) {
            newTimeUnit--;
            _protocol->SendNewTimeUnit(newTimeUnit);
        }
    }
}

void Game::DrawMapUtils() {
    int y = 30;
    raylib->DrawRectangle(800, 10, 200, 180, Fade(LIGHTGRAY, 0.7f));
    raylib->DrawText(("Fréqence : " + std::to_string(newTimeUnit)).c_str(), 810, 20, 20, WHITE);
    raylib->DrawText(("Map Size : " + std::to_string(this->mapSizeW) + "x" + std::to_string(this->mapSizeH)).c_str(), 810, 40, 20, WHITE);
    for (auto &team : teams) {
        raylib->DrawText(("Team " + team.name + " : " + std::to_string(team.players.size())).c_str(), 810, (50 + y), 20, WHITE);
        y += 30;
    }
}

