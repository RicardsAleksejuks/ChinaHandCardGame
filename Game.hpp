#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include "Player.hpp"
#include "Deck.hpp"
#include "Card.hpp"
#include <fstream>

class Game {
public:
    Game();
    void addPlayer(const std::string& name);
    void initialize();
    void start();
    void playTurn();
    void displayState() const;
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename = "");
private:
    std::vector<Player> players;
    Deck deck;
    int currentPlayerIndex;
    std::vector<Card> discardPile;
    bool canPlayCard(const Player& player, const Card& card) const;
    void nextTurn();
    bool checkVictory() const;
    void handlePilePickup(Player& player);
};

#endif