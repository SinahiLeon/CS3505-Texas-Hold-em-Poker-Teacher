#include "card.h"
#include "cardlibrary.h"
#include <QString>

const std::map<QString, Card> CardLibrary::cards = {
    // 13 Spades cards
    {QString("Ace of Spades"), Card(Suit::Spade, CardValue(14))},
    {QString("2 of Spades"), Card(Suit::Spade, CardValue(2))},
    {QString("3 of Spades"), Card(Suit::Spade, CardValue(3))},
    {QString("4 of Spades"), Card(Suit::Spade, CardValue(4))},
    {QString("5 of Spades"), Card(Suit::Spade, CardValue(5))},
    {QString("6 of Spades"), Card(Suit::Spade, CardValue(6))},
    {QString("7 of Spades"), Card(Suit::Spade, CardValue(7))},
    {QString("8 of Spades"), Card(Suit::Spade, CardValue(8))},
    {QString("9 of Spades"), Card(Suit::Spade, CardValue(9))},
    {QString("10 of Spades"), Card(Suit::Spade, CardValue(10))},
    {QString("Jack of Spades"), Card(Suit::Spade, CardValue(11))},
    {QString("Queen of Spades"), Card(Suit::Spade, CardValue(12))},
    {QString("King of Spades"), Card(Suit::Spade, CardValue(13))},
    // 13 Clubs cards
    {QString("Ace of Clubs"), Card(Suit::Club, CardValue(14))},
    {QString("2 of Clubs"), Card(Suit::Club, CardValue(2))},
    {QString("3 of Clubs"), Card(Suit::Club, CardValue(3))},
    {QString("4 of Clubs"), Card(Suit::Club, CardValue(4))},
    {QString("5 of Clubs"), Card(Suit::Club, CardValue(5))},
    {QString("6 of Clubs"), Card(Suit::Club, CardValue(6))},
    {QString("7 of Clubs"), Card(Suit::Club, CardValue(7))},
    {QString("8 of Clubs"), Card(Suit::Club, CardValue(8))},
    {QString("9 of Clubs"), Card(Suit::Club, CardValue(9))},
    {QString("10 of Clubs"), Card(Suit::Club, CardValue(10))},
    {QString("Jack of Clubs"), Card(Suit::Club, CardValue(11))},
    {QString("Queen of Clubs"), Card(Suit::Club, CardValue(12))},
    {QString("King of Clubs"), Card(Suit::Club, CardValue(13))},
    // 13 Hearts cards
    {QString("Ace of Hearts"), Card(Suit::Heart, CardValue(14))},
    {QString("2 of Hearts"), Card(Suit::Heart, CardValue(2))},
    {QString("3 of Hearts"), Card(Suit::Heart, CardValue(3))},
    {QString("4 of Hearts"), Card(Suit::Heart, CardValue(4))},
    {QString("5 of Hearts"), Card(Suit::Heart, CardValue(5))},
    {QString("6 of Hearts"), Card(Suit::Heart, CardValue(6))},
    {QString("7 of Hearts"), Card(Suit::Heart, CardValue(7))},
    {QString("8 of Hearts"), Card(Suit::Heart, CardValue(8))},
    {QString("9 of Hearts"), Card(Suit::Heart, CardValue(9))},
    {QString("10 of Hearts"), Card(Suit::Heart, CardValue(10))},
    {QString("Jack of Hearts"), Card(Suit::Heart, CardValue(11))},
    {QString("Queen of Hearts"), Card(Suit::Heart, CardValue(12))},
    {QString("King of Hearts"), Card(Suit::Heart, CardValue(13))},
    // 13 Diamonds cards
    {QString("Ace of Diamonds"), Card(Suit::Diamond, CardValue(14))},
    {QString("2 of Diamonds"), Card(Suit::Diamond, CardValue(2))},
    {QString("3 of Diamonds"), Card(Suit::Diamond, CardValue(3))},
    {QString("4 of Diamonds"), Card(Suit::Diamond, CardValue(4))},
    {QString("5 of Diamonds"), Card(Suit::Diamond, CardValue(5))},
    {QString("6 of Diamonds"), Card(Suit::Diamond, CardValue(6))},
    {QString("7 of Diamonds"), Card(Suit::Diamond, CardValue(7))},
    {QString("8 of Diamonds"), Card(Suit::Diamond, CardValue(8))},
    {QString("9 of Diamonds"), Card(Suit::Diamond, CardValue(9))},
    {QString("10 of Diamonds"), Card(Suit::Diamond, CardValue(10))},
    {QString("Jack of Diamonds"), Card(Suit::Diamond, CardValue(11))},
    {QString("Queen of Diamonds"), Card(Suit::Diamond, CardValue(12))},
    {QString("King of Diamonds"), Card(Suit::Diamond, CardValue(13))}
};

const QString CardLibrary::cardBack = QString(":/Resources/card_images/back.png");
const QString CardLibrary::noCard = QString(":/Resources/card_images/no_card.png");
