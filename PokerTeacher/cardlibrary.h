#ifndef CARDLIBRARY_H
#define CARDLIBRARY_H

#include "card.h"
#include <QString>
#include <map>
using std::map;

class CardLibrary
{
public:
    /// @brief All card objects in a standard 52 card deck.
    static const map<QString, Card> cards;
    static const QString cardBack;
    static const QString noCard;
};

#endif // CARDLIBRARY_H
