#ifndef CARDLIBRARY_H
#define CARDLIBRARY_H

#include "card.h"
#include <QString>

class CardLibrary
{
public:
    /// @brief All card objects in a standard 52 card deck.
    static const std::vector<Card> cards;
    static const QString cardBack;
    static const QString noCard;
};

#endif // CARDLIBRARY_H
