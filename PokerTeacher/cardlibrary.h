#ifndef CARDLIBRARY_H
#define CARDLIBRARY_H

#include "card.h"
#include <QMap>

class CardLibrary
{
public:
    CardLibrary();
    static const QMap<QString, Card> cards;
};

#endif // CARDLIBRARY_H
