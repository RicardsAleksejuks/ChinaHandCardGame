#include "Player.hpp"
#include "Deck.hpp"
#include <iostream>

Player::Player(const std::string& name) : name(name) {}

void Player::drawCards(Deck& deck, int count) {
    for (int i = 0; i < count; ++i) {
        hand.push_back(deck.drawCard());
    }
}

void Player::drawFaceUpCards(Deck& deck, int count) {
    for (int i = 0; i < count; ++i) {
        faceUpCards.push_back(deck.drawCard());
    }
}

void Player::drawFaceDownCards(Deck& deck, int count) {
    for (int i = 0; i < count; ++i) {
        faceDownCards.push_back(deck.drawCard());
    }
}

void Player::playCard(int index) {
    std::cout << "Attempting to erase card at index " << index
        << " from hand (size: " << hand.size() << ")" << std::endl;

    if (index >= 0 && index < static_cast<int>(hand.size())) {
        hand.erase(hand.begin() + index);
        std::cout << "Card erased successfully!" << std::endl;
    }
    else {
        std::cerr << "Error: Invalid index in playCard(). Hand size: "
            << hand.size() << ", given index: " << index << std::endl;
    }
}


void Player::displayHand() const {
    std::cout << name << "'s hand: ";
    for (const auto& card : hand) {
        std::cout << card.toString() << " ";
    }
    std::cout << std::endl;
}

void Player::addCardToHand(const Card& card) {
    hand.push_back(card);
}

void Player::takeFaceUpCards() {
    if (!faceUpCards.empty()) {
        hand.insert(hand.end(), faceUpCards.begin(), faceUpCards.end());
        faceUpCards.clear();
        std::cout << name << " has taken all face-up cards!" << std::endl;
    }
    else {
        std::cout << name << " has no face-up cards left to take!" << std::endl;
    }
}


void Player::takeFaceDownCard() {
    if (!faceDownCards.empty()) {
        hand.push_back(faceDownCards.back());
        faceDownCards.pop_back();
        std::cout << name << " has taken a face-down card!" << std::endl;
    }
    else {
        std::cout << name << " has no face-down cards left to take!" << std::endl;
    }
}

void Player::setFaceUpCards(const std::vector<Card>& cards) {
    faceUpCards = cards;
}

void Player::setFaceDownCards(const std::vector<Card>& cards) {
    faceDownCards = cards;
}

void Player::addFaceUpCard(const Card& card) {
    faceUpCards.push_back(card);
}

void Player::addFaceDownCard(const Card& card) {
    faceDownCards.push_back(card);
}

void Player::setHand(const std::vector<Card>& cards) {
    hand = cards;
}


const std::vector<Card>& Player::getHand() const {
    return hand;
}

const std::vector<Card>& Player::getFaceUpCards() const {
    return faceUpCards;
}

const std::vector<Card>& Player::getFaceDownCards() const {
    return faceDownCards;
}

const std::string& Player::getName() const {
    return name;
}
