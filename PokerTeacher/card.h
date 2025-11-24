#ifndef CARD_H
#define CARD_H

#include "cardvalue.h"
#include "suit.h"
#include <compare>
#include <QString>
using std::strong_ordering;

class Card
{
public:
    /// @brief Constructs a new Card.
    /// @param suit Suit of the card.
    /// @param value Value of the card.
    Card(QString name, Suit suit, CardValue value, QString image);
    Card& operator=(const Card& other);
    /// @brief Card name ("Ace of Spades", "2 of Clubs", etc.).
    const QString name;
    /// @brief Card suit (Heart, Club, Diamond, Spade).
    const Suit suit;
    /// @brief Card value (Ace, King, Queen, Jack and values 2-10).
    const CardValue value;
    /// @brief Card image from card_images folder.
    const QString image;

    /// @brief This operator inherently generates other comparison operators.
    /// @param other Card to compare to.
    /// @returns Ordering value that can be implicitly cast to bool.
    strong_ordering operator<=>(const Card& other) const;
    /// @brief Defines card equality.
    /// @param other Card to test for equality.
    /// @returns Boolean indicating whether or not cards were equal.
    bool operator==(const Card& other) const;
    bool exactEqual(const Card& other) const;
private:

};

#endif // CARD_H
