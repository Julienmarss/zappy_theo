/*
** EPITECH PROJECT, 2024
** GAME
** File description:
** gui_protocol.cpp
*/

#include "../include/gui_protocol.hpp"

GuiProtocol::GuiProtocol(Game &game) : mapWidth(0), mapHeight(0), gameInstance(game)
{
    commandHandlers = {
            {"msz", &GuiProtocol::handleMapSize},
            {"bct", &GuiProtocol::handleMapTile},
            {"tna", &GuiProtocol::handleTeamName},
            {"ppo", &GuiProtocol::handlePlayerPosition},
            {"plv", &GuiProtocol::handlePlayerLevel},
            {"pin", &GuiProtocol::handlePlayerInventory},
            {"sgt", &GuiProtocol::handleTimeUnitRequest},
            {"pnw", &GuiProtocol::NewPlayer},
            {"pic", &GuiProtocol::handleStartIncantation},
            {"pie", &GuiProtocol::handleEndIncantation},
            {"WELCOME", &GuiProtocol::AuthDone},
            {"pbc", &GuiProtocol::handleBroadcast},
            {"pgt", &GuiProtocol::handleCollecting},
            {"pdi", &GuiProtocol::DeathHandling},
            {"suc", &GuiProtocol::SucHandler},
            {"enw", &GuiProtocol::NewEggHandler},
            {"seg", &GuiProtocol::EndGame},
    };
}

void GuiProtocol::EndGame(const std::vector<std::string>& parameters) {
    if (parameters.size() < 1)
        return;
    std::string winner = parameters[0];
    gameInstance.EndGame(winner);
}

void GuiProtocol::NewEggHandler(const std::vector <std::string> &parameters) {
    if (parameters.size() < 4)
        return;

    int eggId = std::stoi(parameters[0]);
    int playerId = std::stoi(parameters[1]);
    int x = std::stoi(parameters[2]);
    int y = std::stoi(parameters[3]);
    gameInstance.AddEgg(eggId, playerId, x, y);
}

void GuiProtocol::DeathHandling(const std::vector<std::string>& parameters) {
    if (parameters.size() < 1) return;

    int playerId = std::stoi(parameters[0]);
    gameInstance.RemovePlayer(playerId);
}

void GuiProtocol::handleBroadcast(const std::vector<std::string>& parameters) {
    if (parameters.size() < 2) return;

    int playerId = std::stoi(parameters[0]);
    std::string message = parameters[1];
    std::string teamName;
    for (const auto& player : gameInstance.getPlayerInGame()) {
        if (player.id == playerId) {
            teamName = player.teamName;
            break;
        }
    }
    gameInstance.DisplayBroadcastMessage(playerId, message, teamName);
}


void GuiProtocol::handleCollecting(const std::vector<std::string>& parameters) {
    if (parameters.size() < 2)
        return;
    int playerId = std::stoi(parameters[0]);
    int resourceType = std::stoi(parameters[1]);
    std::string message = "Resource collected: " + std::to_string(resourceType);
    std::string teamName;
    for (const auto& player : gameInstance.getPlayerInGame()) {
        if (player.id == playerId) {
            teamName = player.teamName;
            break;
        }
    }
    gameInstance.DisplayCollectingMessage(playerId, message, teamName);
}

void GuiProtocol::handleStartIncantation(const std::vector<std::string>& parameters) {
    if (parameters.size() < 3)
        return;
    int x = std::stoi(parameters[0]);
    int y = std::stoi(parameters[1]);
    for (size_t i = 2; i < parameters.size(); ++i) {
        int playerId = std::stoi(parameters[i]);
        gameInstance.SetPlayerIncantating(playerId, true);
    }
}

void GuiProtocol::handleEndIncantation(const std::vector<std::string>& parameters) {
    if (parameters.size() < 3)
        return;

    int x = std::stoi(parameters[0]);
    int y = std::stoi(parameters[1]);
    for (auto& player : gameInstance.getPlayerInGame())  {
        if (player.position.x == x && player.position.z == y) {
            gameInstance.SetPlayerIncantating(player.id, false);
        }
    }
}

void GuiProtocol::AuthDone(const std::vector<std::string>& parameters)
{
    return;
}

void GuiProtocol::NewPlayer(const std::vector<std::string>& parameters) {
    if (parameters.size() < 6) return;

    int id = std::stoi(parameters[0]);
    int x = std::stoi(parameters[1]);
    int y = std::stoi(parameters[2]);
    int orientation = std::stoi(parameters[3]);
    int level = std::stoi(parameters[4]);
    std::string teamName = parameters[5];

    Vector3 position = { static_cast<float>(x), 0.0f, static_cast<float>(y) };
    Player newPlayer(id, position, orientation, level, teamName, gameInstance.raylib);

    static std::unordered_map<std::string, Color> teamColors;
    if (teamColors.find(teamName) == teamColors.end()) {
        teamColors[teamName] = gameInstance.GenerateRandomColor();
    }
    newPlayer.SetColor(teamColors[teamName]);
    gameInstance.AddPlayer(newPlayer);
}

void GuiProtocol::Connect(const std::string& host, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address / Address not supported");
    }
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Connection failed");
    }
}

void GuiProtocol::Authenticate()
{
    std::string authMessage = "GRAPHIC\n";
    ssize_t sentBytes = send(sockfd, authMessage.c_str(), strlen(authMessage.c_str()), 0);
    if (sentBytes != static_cast<ssize_t>(strlen(authMessage.c_str()))) {
        throw std::runtime_error("Failed to send authentication message");
    }
    std::cout << "Authentication message sent successfully: " << authMessage.c_str() << std::endl;
}

void GuiProtocol::ProcessIncomingData()
{
    char ch;
    std::string buffer;
    bool initialDataReceived = false;

    while (read(sockfd, &ch, 1) > 0) {
        buffer += ch;
        if (ch == '\n') {
            ParseMessage(buffer);
            buffer.clear();
            if (!initialDataReceived) {
                if (mapWidth > 0 && mapHeight > 0 && !teamNames.empty() && !mapTiles.empty()) {
                    initialDataReceived = true;
                    std::cout << "Initial data received successfully\n";
                }
            }
        }
    }
}

void GuiProtocol::ParseMessage(const std::string& message)
{
    std::istringstream iss(message);
    std::string command;
    std::vector<std::string> parameters;
    std::string token;

    iss >> command;
    while (iss >> token) {
        parameters.push_back(token);
    }
    auto it = commandHandlers.find(command);
    if (it != commandHandlers.end()) {
        try {
            std::invoke(it->second, this, parameters);
        } catch (const std::exception& e) {
            std::cerr << "Failed to handle command: " << command << " - " << e.what() << std::endl;
        }
    }
}

void GuiProtocol::handleMapSize(const std::vector<std::string>& parameters)
{
    if (parameters.size() < 2) {
        throw ProtocolException("Invalid number of parameters for msz command");
        return;
    }
    mapWidth = std::stoi(parameters[0]);
    mapHeight = std::stoi(parameters[1]);
}

void GuiProtocol::handleMapTile(const std::vector<std::string>& parameters)
{
    if (parameters.size() < 9) {
        throw ProtocolException("Invalid number of parameters for bct command");
        return;
    }

    int x = std::stoi(parameters[0]);
    int y = std::stoi(parameters[1]);
    MapTile tile;
    tile.x = x;
    tile.y = y;
    tile.resources[0] = std::stoi(parameters[2]);
    tile.resources[1] = std::stoi(parameters[3]);
    tile.resources[2] = std::stoi(parameters[4]);
    tile.resources[3] = std::stoi(parameters[5]);
    tile.resources[4] = std::stoi(parameters[6]);
    tile.resources[5] = std::stoi(parameters[7]);
    tile.resources[6] = std::stoi(parameters[8]);
    mapTiles[x + y * mapWidth] = tile;
}

void GuiProtocol::handleTeamName(const std::vector<std::string>& parameters)
{
    if (parameters.empty()) {
        throw ProtocolException("Invalid number of parameters for tna command");
        return;
    }
    teamNames.push_back(parameters[0]);
}

void GuiProtocol::handlePlayerPosition(const std::vector<std::string>& parameters) {
    if (parameters.size() < 4)
        return;

    int id = std::stoi(parameters[0]);
    int x = std::stoi(parameters[1]);
    int y = std::stoi(parameters[2]);
    int orientation = std::stoi(parameters[3]);
    Vector3 newPosition = { static_cast<float>(x), 0.0f, static_cast<float>(y) };
    gameInstance.UpdatePlayerPosition(id, newPosition, orientation);
}

void GuiProtocol::handlePlayerLevel(const std::vector<std::string>& parameters)
{
    if (parameters.size() < 2) {
        throw ProtocolException("Invalid number of parameters for plv command");
        return;
    }
    int id = std::stoi(parameters[0]);
    int level = std::stoi(parameters[1]);
    gameInstance.UpdatePlayerLevel(id, level);
}

void GuiProtocol::handlePlayerInventory(const std::vector<std::string>& parameters)
{
    if (parameters.size() < 9) {
        throw ProtocolException("Invalid number of parameters for pin command");
        return;
    }

    int id = std::stoi(parameters[0]);
    int x = std::stoi(parameters[1]);
    int y = std::stoi(parameters[2]);
    std::array<int, 7> inventory = {
        std::stoi(parameters[3]), std::stoi(parameters[4]), std::stoi(parameters[5]),
        std::stoi(parameters[6]), std::stoi(parameters[7]), std::stoi(parameters[8]),
        std::stoi(parameters[9])
    };
    gameInstance.UpdatePlayerInventory(id, inventory);
}

void GuiProtocol::handleTimeUnitRequest(const std::vector<std::string>& parameters)
{
    timeUnit = std::stoi(parameters[0]);
    gameInstance.SetMapUtils(timeUnit, mapWidth, mapHeight);
}

void GuiProtocol::RequestPlayerInventory(int playerId) {
    std::string command = "pin " + std::to_string(playerId) + "\n";
    send(sockfd, command.c_str(), command.size(), 0);
    std::string command2 = "plv " + std::to_string(playerId) + "\n";
    send(sockfd, command2.c_str(), command2.size(), 0);
}

void GuiProtocol::SendNewTimeUnit(int newTimeUnit) {
    std::string command = "sst " + std::to_string(newTimeUnit) + "\n";
    send(sockfd, command.c_str(), command.size(), 0);
}
