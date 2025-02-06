#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include "Card.hpp"
#include "Deck.hpp"

class Player {
public:
    Player(const std::string& name);

    void drawCards(Deck& deck, int count);
    void drawFaceUpCards(Deck& deck, int count);
    void drawFaceDownCards(Deck& deck, int count);
    void playCard(int index);
    void displayHand() const;
    void addCardToHand(const Card& card);
    void takeFaceUpCards();
    void takeFaceDownCard();

    void setFaceUpCards(const std::vector<Card>& cards);
    void setFaceDownCards(const std::vector<Card>& cards);
    void addFaceUpCard(const Card& card);
    void addFaceDownCard(const Card& card);
    void setHand(const std::vector<Card>& cards);

    const std::vector<Card>& getHand() const;
    const std::vector<Card>& getFaceUpCards() const;
    const std::vector<Card>& getFaceDownCards() const;
    const std::string& getName() const;

private:
    std::string name;
    std::vector<Card> hand;
    std::vector<Card> faceUpCards;
    std::vector<Card> faceDownCards;
};

#endif
