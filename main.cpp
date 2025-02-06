#include <iostream>
#include <limits>
#include "Game.hpp"

int main() {
    try {
        std::cout << "Welcome to the game 'China Hand'!\n";
        Game game;
        std::string input;

        std::cout << "Do you want to load a saved game? (yes/no): ";
        std::cin >> input;

        if (input == "yes") {
            if (game.loadGame()) {
                std::cout << "Loaded saved game successfully!\n";
                game.start();
            }
            else {
                std::cout << "Failed to load game. Starting a new one...\n";
                input = "no";
            }
        }

        if (input != "yes" || input == "no") {
            int playerCount;
            do {
                std::cout << "Enter the number of players (minimum 2): ";
                std::cin >> playerCount;

                if (!std::cin) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid input! Please enter a number.\n";
                }
                else if (playerCount < 2) {
                    std::cerr << "The game requires at least 2 players!\n";
                }
            } while (playerCount < 2 || !std::cin);

            for (int i = 1; i <= playerCount; ++i) {
                std::string playerName;
                std::cout << "Enter player name " << i << ": ";
                std::cin >> playerName;
                game.addPlayer(playerName);
            }

            std::cout << "\nInitializing the game...\n";
            game.initialize();
            std::cout << "\nStarting the game!\n";
            game.start();
        }

        std::cout << "Do you want to save the game? (yes/no): ";
        std::cin >> input;
        if (input == "yes") {
            std::string saveName;
            std::cout << "Enter save file name (without .sav extension): ";
            std::cin >> saveName;
            game.saveGame(saveName);
        }

        std::cout << "\nThe game has ended. Thanks for playing!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return 1;
    }
    return 0;
}