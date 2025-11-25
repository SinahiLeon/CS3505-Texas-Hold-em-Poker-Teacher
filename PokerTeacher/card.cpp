#include "card.h"

Card::Card(Suit suit, CardValue value, QString image)
    : suit(suit)
    , value(value)
    , image(image)
{

}

strong_ordering Card::operator<=>(const Card& other) const {
    return value <=> other.value;
}

bool Card::operator==(const Card& other) const {
    return value == other.value;
}

bool Card::exactEqual(const Card& other) const {
    return value == other.value && suit == other.suit;
}
