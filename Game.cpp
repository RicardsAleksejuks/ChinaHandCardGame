#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>  
#include <vector>
#include <filesystem>
#include <sstream>

Game::Game() : currentPlayerIndex(0) {}

void Game::addPlayer(const std::string& name) {
    players.emplace_back(name);
}

void Game::initialize() {
    deck.shuffle();
    for (auto& player : players) {
        player.drawCards(deck, 3);
        player.drawFaceUpCards(deck, 3);
        player.drawFaceDownCards(deck, 3);
    }
}

void Game::start() {
    std::cout << "Game has started!" << std::endl;

    while (!checkVictory()) {
        playTurn();
        nextTurn();
    }

    for (const Player& player : players) {
        if (player.getHand().empty() && player.getFaceUpCards().empty() && player.getFaceDownCards().empty()) {
            std::cout << player.getName() << " wins!" << std::endl;
            break;
        }
    }
}

void Game::playTurn() {
    Player& currentPlayer = players[currentPlayerIndex];
    std::cout << currentPlayer.getName() << "'s turn." << std::endl;
    displayState();

    bool cardPlayed = false;
    while (!cardPlayed) {
        if (currentPlayer.getHand().empty()) {
            if (!currentPlayer.getFaceUpCards().empty()) {
                currentPlayer.takeFaceUpCards();
                std::cout << currentPlayer.getName() << " takes face-up cards!" << std::endl;
            }
            else if (!currentPlayer.getFaceDownCards().empty()) {
                std::cout << currentPlayer.getName() << " draws a face-down card!" << std::endl;
                currentPlayer.takeFaceDownCard();
            }
            else {
                std::cout << currentPlayer.getName() << " has no more cards!" << std::endl;
                return;
            }
        }

        if (std::none_of(currentPlayer.getHand().begin(), currentPlayer.getHand().end(),
            [this, &currentPlayer](const Card& card) { return canPlayCard(currentPlayer, card); })) {
            handlePilePickup(currentPlayer);
            return;
        }

        int index;
        while (true) {
            std::cout << "Choose a card to play (index from 1 to " << currentPlayer.getHand().size() << ", or 0 to save): ";
            std::cin >> index;

            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter a number.\n";
            }
            else if (index == 0) {
                std::cout << "Enter filename to save: ";
                std::string filename;
                std::cin >> filename;
                if (saveGame(filename)) {
                    std::cout << "Game saved successfully!\n";
                }
                else {
                    std::cout << "Failed to save the game.\n";
                }
                break;
            }
            else if (index > 0 && index <= currentPlayer.getHand().size()) {
                index--;
                Card selectedCard = currentPlayer.getHand()[index];

                if (canPlayCard(currentPlayer, selectedCard)) {
                    currentPlayer.playCard(index);
                    discardPile.push_back(selectedCard);
                    cardPlayed = true;

                    if (selectedCard.getValue() == 2) {
                        std::cout << "You played a 2. Play an additional card!" << std::endl;
                    }
                    else if (selectedCard.getValue() == 10) {
                        std::cout << "You played a 10. The discard pile is reset!" << std::endl;
                        discardPile.clear();
                    }
                    else {
                        std::cout << "Card played: " << selectedCard.toString() << std::endl;
                    }

                    if (currentPlayer.getHand().size() <= 2) {
                        if (!deck.isEmpty()) {
                            currentPlayer.drawCards(deck, 1);
                        }
                        else if (!currentPlayer.getFaceUpCards().empty()) {
                            currentPlayer.takeFaceUpCards();
                        }
                        else if (!currentPlayer.getFaceDownCards().empty()) {
                            currentPlayer.takeFaceDownCard();
                        }
                    }
                }
                else {
                    std::cout << "Invalid card. Try again!" << std::endl;
                }
                break;
            }
            else {
                std::cout << "Invalid index! Try again!" << std::endl;
            }
        }
    }
}


void Game::handlePilePickup(Player& currentPlayer) {
    if (!discardPile.empty()) {
        std::cout << currentPlayer.getName() << " cannot play a valid card and must pick up the pile!" << std::endl;
        for (const Card& card : discardPile) {
            currentPlayer.addCardToHand(card);
        }
        discardPile.clear();
    }
    else {
        std::cout << currentPlayer.getName() << " cannot play a valid card and must draw face-up cards!" << std::endl;
        if (!currentPlayer.getFaceUpCards().empty()) {
            currentPlayer.drawFaceUpCards(deck, 3);
        }
        else {
            std::cout << "No more face-up cards available. The game cannot continue until the deck is re-shuffled." << std::endl;
        }
    }
}

void Game::displayState() const {
    std::cout << "Current Discard Pile: ";
    for (const auto& card : discardPile) {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
    std::cout << "Player hands: " << std::endl;
    for (const auto& player : players) {
        player.displayHand();
    }
}

bool Game::canPlayCard(const Player& player, const Card& card) const {
    if (discardPile.empty()) return true;
    const Card& topCard = discardPile.back();
    return card.getValue() >= topCard.getValue() || card.getValue() == 2;
}

void Game::nextTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

bool Game::checkVictory() const {
    int playersWithCards = 0;
    Player* potentialWinner = nullptr;

    for (const Player& player : players) {
        if (!player.getHand().empty() || !player.getFaceUpCards().empty() || !player.getFaceDownCards().empty()) {
            playersWithCards++;
        }
        else {
            potentialWinner = const_cast<Player*>(&player);
        }
    }

    return (playersWithCards == 1 && potentialWinner != nullptr);
}

bool Game::saveGame(const std::string& filename) const {
    std::string saveFilename = filename;

    if (saveFilename.empty()) {
        std::cout << "Enter filename to save: ";
        std::getline(std::cin, saveFilename);
    }

    if (saveFilename.length() < 4 || saveFilename.substr(saveFilename.length() - 4) != ".sav") {
        saveFilename += ".sav";
    }

    std::ofstream outFile(saveFilename);
    if (!outFile) {
        std::cerr << "Error: Unable to create save file " << saveFilename << std::endl;
        return false;
    }

    try {
        outFile << players.size() << "\n";
        for (const auto& player : players) {
            outFile << player.getName() << "\n";

            outFile << player.getHand().size() << " ";
            for (const auto& card : player.getHand()) {
                outFile << card.getValue() << " ";
            }
            outFile << "\n";

            outFile << player.getFaceUpCards().size() << " ";
            for (const auto& card : player.getFaceUpCards()) {
                outFile << card.getValue() << " ";
            }
            outFile << "\n";

            outFile << player.getFaceDownCards().size() << " ";
            for (const auto& card : player.getFaceDownCards()) {
                outFile << card.getValue() << " ";
            }
            outFile << "\n";
        }

        outFile << discardPile.size() << " ";
        for (const auto& card : discardPile) {
            outFile << card.getValue() << " ";
        }
        outFile << "\n";

        outFile << currentPlayerIndex << "\n";

        std::cout << "Game saved successfully to " << saveFilename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error while saving game: " << e.what() << std::endl;
        return false;
    }
}


bool Game::loadGame(const std::string& filename) {
    std::string loadFilename = filename;

    if (loadFilename.length() < 10) {
        std::cout << "Enter filename to load: ";
        std::cin >> loadFilename;
        deck.shuffle();
        for (auto& player : players) {
            player.drawCards(deck, 3);
            player.drawFaceUpCards(deck, 3);
            player.drawFaceDownCards(deck, 3);
        }
    }

    std::ifstream inFile(loadFilename + ".sav");
    if (!inFile) {
        std::cerr << "Error: Unable to open save file " << loadFilename << std::endl;
        return false;
    }

    try {

        std::string line;

        if (!std::getline(inFile, line)) {
            std::cerr << "Error: Unable to read number of players" << std::endl;
            return false;
        }
        int numPlayers = std::stoi(line);

        for (int i = 0; i < numPlayers; ++i) {

            if (!std::getline(inFile, line)) {
                std::cerr << "Error: Unable to read player name" << std::endl;
                return false;
            }
            players.emplace_back(line);

            if (!std::getline(inFile, line)) {
                std::cerr << "Error: Unable to read hand cards" << std::endl;
                return false;
            }
            std::istringstream handStream(line);
            int handSize;
            handSize = std::stoi(line.substr(0, line.find(' ')));
            std::vector<Card> hand;
            for (int j = 0; j < handSize; ++j) {
                size_t pos = line.find(' ');
                if (pos == std::string::npos) break;
                line = line.substr(pos + 1);
                hand.emplace_back(std::stoi(line.substr(0, line.find(' '))));
            }
            players[i].setHand(hand);

            if (!std::getline(inFile, line)) {
                std::cerr << "Error: Unable to read face-up cards" << std::endl;
                return false;
            }
            int faceUpSize = std::stoi(line.substr(0, line.find(' ')));
            std::vector<Card> faceUpCards;
            line = line.substr(line.find(' ') + 1);
            for (int j = 0; j < faceUpSize; ++j) {
                size_t pos = line.find(' ');
                faceUpCards.emplace_back(std::stoi(line.substr(0, pos)));
                if (pos == std::string::npos) break;
                line = line.substr(pos + 1);
            }
            players[i].setFaceUpCards(faceUpCards);

            if (!std::getline(inFile, line)) {
                std::cerr << "Error: Unable to read face-down cards" << std::endl;
                return false;
            }
            int faceDownSize = std::stoi(line.substr(0, line.find(' ')));
            std::vector<Card> faceDownCards;
            line = line.substr(line.find(' ') + 1);
            for (int j = 0; j < faceDownSize; ++j) {
                size_t pos = line.find(' ');
                faceDownCards.emplace_back(std::stoi(line.substr(0, pos)));
                if (pos == std::string::npos) break;
                line = line.substr(pos + 1);
            }
            players[i].setFaceDownCards(faceDownCards);
        }

        if (!std::getline(inFile, line)) {
            std::cerr << "Error: Unable to read discard pile" << std::endl;
            return false;
        }
        int discardSize = std::stoi(line.substr(0, line.find(' ')));
        line = line.substr(line.find(' ') + 1);
        for (int i = 0; i < discardSize; ++i) {
            size_t pos = line.find(' ');
            discardPile.emplace_back(std::stoi(line.substr(0, pos)));
            if (pos == std::string::npos) break;
            line = line.substr(pos + 1);
        }

        if (!std::getline(inFile, line)) {
            std::cerr << "Error: Unable to read current player index" << std::endl;
            return false;
        }
        currentPlayerIndex = std::stoi(line);

        std::cout << "Game loaded successfully from " << loadFilename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading save file: " << e.what() << std::endl;
        return false;
    }
}






