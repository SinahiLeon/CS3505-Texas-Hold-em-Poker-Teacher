#include "card.h"

Card::Card(Suit suit, CardValue value) :
    suit(suit),
    value(value)
{}

strong_ordering Card::operator<=>(const Card& other) const {
    return value <=> other.value;
}

bool Card::operator==(const Card& other) const {
    return value == other.value;
}
