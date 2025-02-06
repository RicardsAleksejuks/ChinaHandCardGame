#include "Deck.hpp"
#include <algorithm>
#include <random>
#include <ctime>
#include <stdexcept>

Deck::Deck() : currentCardIndex(0) {
    for (int i = 2; i <= 10; ++i) {
        for (int j = 0; j < 4; ++j) {
            cards.push_back(Card(i));
        }
    }
    cards.push_back(Card(2));
    cards.push_back(Card(10));
}

void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(static_cast<unsigned int>(std::time(0))));
}

Card Deck::drawCard() {
    if (currentCardIndex < cards.size()) {
        return cards[currentCardIndex++];
    }
    else {
        throw std::out_of_range("Deck is empty");
    }
}

bool Deck::isEmpty() const {
    return currentCardIndex >= cards.size();
}
