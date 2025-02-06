#include "Card.hpp"
#include <sstream>

Card::Card(int value) : value(value) {}

int Card::getValue() const {
    return value;
}

std::string Card::toString() const {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}
