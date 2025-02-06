#ifndef DECK_HPP
#define DECK_HPP

#include <vector>
#include "Card.hpp"

class Deck {
public:
    Deck();
    void shuffle();
    Card drawCard();
    bool isEmpty() const;

private:
    std::vector<Card> cards;
    int currentCardIndex;
};

#endif
