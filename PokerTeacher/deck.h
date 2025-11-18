#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <queue>

class Deck
{
public:
    Deck();

private:
    std::queue<Card> cards;
};

#endif // DECK_H
