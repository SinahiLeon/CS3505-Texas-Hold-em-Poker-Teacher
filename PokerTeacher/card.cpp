#include "card.h"
#include <QRegularExpression>

Card::Card(QString name, Suit suit, CardValue value, QString image)
    : name(name)
    , suit(suit)
    , value(value)
    , image(image)
{

}

Card::Card(Suit suit, CardValue value, QString image)
    : name(toString(suit, value))
    , suit(suit)
    , value(value)
    , image(image)
{

}

Card::Card(Suit suit, CardValue value)
    : name(toString(suit, value))
    , suit(suit)
    , value(value)
    , image(toFilePath(suit, value))
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
    return toString(suit, value);
}

QString Card::toString(Suit cardSuit, CardValue cardValue) const {
    QString valueString;
    QString of = " of ";
    QString suitString;
    switch(cardValue) {
    case CardValue::Ace:
        valueString = "Ace";
        break;
    case CardValue::Two:
        valueString = "2";
        break;
    case CardValue::Three:
        valueString = "3";
        break;
    case CardValue::Four:
        valueString = "4";
        break;
    case CardValue::Five:
        valueString = "5";
        break;
    case CardValue::Six:
        valueString = "6";
        break;
    case CardValue::Seven:
        valueString = "7";
        break;
    case CardValue::Eight:
        valueString = "8";
        break;
    case CardValue::Nine:
        valueString = "9";
        break;
    case CardValue::Ten:
        valueString = "10";
        break;
    case CardValue::Jack:
        valueString = "Jack";
        break;
    case CardValue::Queen:
        valueString = "Queen";
        break;
    case CardValue::King:
        valueString = "King";
        break;
    default:
        valueString = "ERROR";
    }
    switch(cardSuit) {
    case Suit::Club:
        suitString = "Clubs";
        break;
    case Suit::Diamond:
        suitString = "Diamonds";
        break;
    case Suit::Heart:
        suitString = "Hearts";
        break;
    case Suit::Spade:
        suitString = "Spades";
        break;
    default:
        suitString = "ERROR";
    }
    return valueString+of+suitString;
}

QString Card::toFilePath() const {
    return toFilePath(suit, value);
}

QString Card::toFilePath(Suit cardSuit, CardValue cardValue) const {
    QString basePath = ":/Resources/card_images/%1.png";

    // Format card name
    QString name = toString(cardSuit, cardValue);
    name = name.toLower();
    QRegularExpression getSpaces(" ");
    name = name.replace(getSpaces, "_");

    return basePath.arg(name);
}
