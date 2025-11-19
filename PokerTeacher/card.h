#ifndef CARD_H
#define CARD_H

#include "cardvalue.h"
#include "suit.h"
#include <compare>
#include <qimage.h>
using std::strong_ordering;

class Card
{
public:
    /// @brief Constructs a new Card.
    /// @param suit Suit of the card.
    /// @param value Value of the card.
    Card(Suit suit, CardValue value, QImage image);
    /// @brief Card suit (Heart, Club, Diamond, Spade).
    const Suit suit;
    /// @brief Card value (Ace, King, Queen, Jack and values 2-10).
    const CardValue value;
    /// @brief Card image from card_images folder.
    const QImage image;

    /// @brief This operator inherently generates other comparison operators.
    /// @param other Card to compare to.
    /// @returns Ordering value that can be implicitly cast to bool.
    strong_ordering operator<=>(const Card& other) const;
    /// @brief Defines card equality.
    /// @param other Card to test for equality.
    /// @returns Boolean indicating whether or not cards were equal.
    /// @remarks This operator is required for <=> to auto-generate other operators.
    bool operator==(const Card& other) const;
};

#endif // CARD_H
