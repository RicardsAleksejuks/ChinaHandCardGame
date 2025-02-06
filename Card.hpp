#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card {
public:
    Card(int value);

    int getValue() const;
    std::string toString() const;

private:
    int value;
};

#endif
