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

QString Card::toString() const {
    QString name = " of ";
    switch(value) {
    case CardValue::Ace:
        name = "Ace"+name;
        break;
    case CardValue::Two:
        name = "2"+name;
        break;
    case CardValue::Three:
        name = "3"+name;
        break;
    case CardValue::Four:
        name = "4"+name;
        break;
    case CardValue::Five:
        name = "5"+name;
        break;
    case CardValue::Six:
        name = "6"+name;
        break;
    case CardValue::Seven:
        name = "7"+name;
        break;
    case CardValue::Eight:
        name = "8"+name;
        break;
    case CardValue::Nine:
        name = "9"+name;
        break;
    case CardValue::Ten:
        name = "10"+name;
        break;
    case CardValue::Jack:
        name = "Jack"+name;
        break;
    case CardValue::Queen:
        name = "Queen"+name;
        break;
    case CardValue::King:
        name = "King"+name;
        break;
    default:
        name = name+"ERROR";
    }
    switch(suit) {
    case Suit::Club:
        name = name+"Clubs";
        break;
    case Suit::Diamond:
        name = name+"Diamonds";
        break;
    case Suit::Heart:
        name = name+"Hearts";
        break;
    case Suit::Spade:
        name = name+"Spades";
        break;
    default:
        name = name+"ERROR";
    }
    return name;
}
