/*
** EPITECH PROJECT, 2024
** GUI
** File description:
** main.cpp
*/
#include "gui_protocol.hpp"

int main(int argc, char *argv[]) {
    int option;
    std::string hostArg;
    int portArg = 0;
    while ((option = getopt(argc, argv, "h:p:")) != -1) {
        switch (option) {
            case 'h':
                hostArg = optarg;
                break;
            case 'p':
                portArg = std::stoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -p port -h machine" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
    if (hostArg.empty() || portArg == 0) {
        std::cerr << "Usage: " << argv[0] << " -p port -h machine" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::shared_ptr<RaylibWrapper> raylib = std::make_shared<RaylibWrapper>(1000, 800, "ZAPPY");
    Game game(raylib);
    auto protocol = std::make_shared<GuiProtocol>(game);
    protocol->assign_raylib(raylib);
    try {
        protocol->Connect(hostArg, portArg);
        protocol->Authenticate();
        std::thread(&GuiProtocol::ProcessIncomingData, protocol).detach();
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect to server: " << e.what() << std::endl;
        return 84;
    }
    try {
        game.Run(protocol);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}
