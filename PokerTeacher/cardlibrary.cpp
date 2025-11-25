#include "card.h"
#include "cardlibrary.h"
#include <QString>

const std::map<QString, Card> CardLibrary::cards = {
    // 13 Spades cards
    {QString("Ace of Spades"), Card(Suit::Spade, CardValue(14), QString(":/Resources/card_images/ace_of_spades.png"))},
    {QString("2 of Spades"), Card(Suit::Spade, CardValue(2), QString(":/Resources/card_images/2_of_spades.png"))},
    {QString("3 of Spades"), Card(Suit::Spade, CardValue(3), QString(":/Resources/card_images/3_of_spades.png"))},
    {QString("4 of Spades"), Card(Suit::Spade, CardValue(4), QString(":/Resources/card_images/4_of_spades.png"))},
    {QString("5 of Spades"), Card(Suit::Spade, CardValue(5), QString(":/Resources/card_images/5_of_spades.png"))},
    {QString("6 of Spades"), Card(Suit::Spade, CardValue(6), QString(":/Resources/card_images/6_of_spades.png"))},
    {QString("7 of Spades"), Card(Suit::Spade, CardValue(7), QString(":/Resources/card_images/7_of_spades.png"))},
    {QString("8 of Spades"), Card(Suit::Spade, CardValue(8), QString(":/Resources/card_images/8_of_spades.png"))},
    {QString("9 of Spades"), Card(Suit::Spade, CardValue(9), QString(":/Resources/card_images/9_of_spades.png"))},
    {QString("10 of Spades"), Card(Suit::Spade, CardValue(10), QString(":/Resources/card_images/10_of_spades.png"))},
    {QString("Jack of Spades"), Card(Suit::Spade, CardValue(11), QString(":/Resources/card_images/jack_of_spades.png"))},
    {QString("Queen of Spades"), Card(Suit::Spade, CardValue(12), QString(":/Resources/card_images/queen_of_spades.png"))},
    {QString("King of Spades"), Card(Suit::Spade, CardValue(13), QString(":/Resources/card_images/king_of_spades.png"))},
    // 13 Clubs cards
    {QString("Ace of Clubs"), Card(Suit::Club, CardValue(14), QString(":/Resources/card_images/ace_of_clubs.png"))},
    {QString("2 of Clubs"), Card(Suit::Club, CardValue(2), QString(":/Resources/card_images/2_of_clubs.png"))},
    {QString("3 of Clubs"), Card(Suit::Club, CardValue(3), QString(":/Resources/card_images/3_of_clubs.png"))},
    {QString("4 of Clubs"), Card(Suit::Club, CardValue(4), QString(":/Resources/card_images/4_of_clubs.png"))},
    {QString("5 of Clubs"), Card(Suit::Club, CardValue(5), QString(":/Resources/card_images/5_of_clubs.png"))},
    {QString("6 of Clubs"), Card(Suit::Club, CardValue(6), QString(":/Resources/card_images/6_of_clubs.png"))},
    {QString("7 of Clubs"), Card(Suit::Club, CardValue(7), QString(":/Resources/card_images/7_of_clubs.png"))},
    {QString("8 of Clubs"), Card(Suit::Club, CardValue(8), QString(":/Resources/card_images/8_of_clubs.png"))},
    {QString("9 of Clubs"), Card(Suit::Club, CardValue(9), QString(":/Resources/card_images/9_of_clubs.png"))},
    {QString("10 of Clubs"), Card(Suit::Club, CardValue(10), QString(":/Resources/card_images/10_of_clubs.png"))},
    {QString("Jack of Clubs"), Card(Suit::Club, CardValue(11), QString(":/Resources/card_images/jack_of_clubs.png"))},
    {QString("Queen of Clubs"), Card(Suit::Club, CardValue(12), QString(":/Resources/card_images/queen_of_clubs.png"))},
    {QString("King of Clubs"), Card(Suit::Club, CardValue(13), QString(":/Resources/card_images/king_of_clubs.png"))},
    // 13 Hearts cards
    {QString("Ace of Hearts"), Card(Suit::Heart, CardValue(14), QString(":/Resources/card_images/ace_of_hearts.png"))},
    {QString("2 of Hearts"), Card(Suit::Heart, CardValue(2), QString(":/Resources/card_images/2_of_hearts.png"))},
    {QString("3 of Hearts"), Card(Suit::Heart, CardValue(3), QString(":/Resources/card_images/3_of_hearts.png"))},
    {QString("4 of Hearts"), Card(Suit::Heart, CardValue(4), QString(":/Resources/card_images/4_of_hearts.png"))},
    {QString("5 of Hearts"), Card(Suit::Heart, CardValue(5), QString(":/Resources/card_images/5_of_hearts.png"))},
    {QString("6 of Hearts"), Card(Suit::Heart, CardValue(6), QString(":/Resources/card_images/6_of_hearts.png"))},
    {QString("7 of Hearts"), Card(Suit::Heart, CardValue(7), QString(":/Resources/card_images/7_of_hearts.png"))},
    {QString("8 of Hearts"), Card(Suit::Heart, CardValue(8), QString(":/Resources/card_images/8_of_hearts.png"))},
    {QString("9 of Hearts"), Card(Suit::Heart, CardValue(9), QString(":/Resources/card_images/9_of_hearts.png"))},
    {QString("10 of Hearts"), Card(Suit::Heart, CardValue(10), QString(":/Resources/card_images/10_of_hearts.png"))},
    {QString("Jack of Hearts"), Card(Suit::Heart, CardValue(11), QString(":/Resources/card_images/jack_of_hearts.png"))},
    {QString("Queen of Hearts"), Card(Suit::Heart, CardValue(12), QString(":/Resources/card_images/queen_of_hearts.png"))},
    {QString("King of Hearts"), Card(Suit::Heart, CardValue(13), QString(":/Resources/card_images/king_of_hearts.png"))},
    // 13 Diamonds cards
    {QString("Ace of Diamonds"), Card(Suit::Diamond, CardValue(14), QString(":/Resources/card_images/ace_of_diamonds.png"))},
    {QString("2 of Diamonds"), Card(Suit::Diamond, CardValue(2), QString(":/Resources/card_images/2_of_diamonds.png"))},
    {QString("3 of Diamonds"), Card(Suit::Diamond, CardValue(3), QString(":/Resources/card_images/3_of_diamonds.png"))},
    {QString("4 of Diamonds"), Card(Suit::Diamond, CardValue(4), QString(":/Resources/card_images/4_of_diamonds.png"))},
    {QString("5 of Diamonds"), Card(Suit::Diamond, CardValue(5), QString(":/Resources/card_images/5_of_diamonds.png"))},
    {QString("6 of Diamonds"), Card(Suit::Diamond, CardValue(6), QString(":/Resources/card_images/6_of_diamonds.png"))},
    {QString("7 of Diamonds"), Card(Suit::Diamond, CardValue(7), QString(":/Resources/card_images/7_of_diamonds.png"))},
    {QString("8 of Diamonds"), Card(Suit::Diamond, CardValue(8), QString(":/Resources/card_images/8_of_diamonds.png"))},
    {QString("9 of Diamonds"), Card(Suit::Diamond, CardValue(9), QString(":/Resources/card_images/9_of_diamonds.png"))},
    {QString("10 of Diamonds"), Card(Suit::Diamond, CardValue(10), QString(":/Resources/card_images/10_of_diamonds.png"))},
    {QString("Jack of Diamonds"), Card(Suit::Diamond, CardValue(11), QString(":/Resources/card_images/jack_of_diamonds.png"))},
    {QString("Queen of Diamonds"), Card(Suit::Diamond, CardValue(12), QString(":/Resources/card_images/queen_of_diamonds.png"))},
    {QString("King of Diamonds"), Card(Suit::Diamond, CardValue(13), QString(":/Resources/card_images/king_of_diamonds.png"))}
};

const QString CardLibrary::cardBack = QString(":/Resources/card_images/back.png");
const QString CardLibrary::noCard = QString(":/Resources/card_images/no_card.png");
