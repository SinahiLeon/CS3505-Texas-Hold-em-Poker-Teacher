#ifndef CARDLIBRARY_H
#define CARDLIBRARY_H

#include "card.h"
#include <map>

class CardLibrary
{
public:
    CardLibrary();
    /// @brief All card objects in a standard 52 card deck.
    static const std::map<QString, Card> cards;
};

#endif // CARDLIBRARY_H
