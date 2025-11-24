#include "card.h"

Card::Card(QString name, Suit suit, CardValue value, QString image)
    : name(name)
    , suit(suit)
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
