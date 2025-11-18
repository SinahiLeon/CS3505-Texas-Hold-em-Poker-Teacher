#ifndef CARDHAND_H
#define CARDHAND_H

#include <compare>
#include <vector>
#include "card.h"
#include "handtype.h"
using std::strong_ordering;
using std::vector;

class CardHand
{
public:
    CardHand();
    strong_ordering operator<=>(const CardHand& other) const;
    bool operator==(const Card& other) const;
    void recieveCards(vector<Card> playerCards);
    Card getHighCard();
    vector<Card> getKickers();
private:
    vector<Card> cards;
    HandType hand = HandType::Empty;

    void evaluateHandType();
};

#endif // CARDHAND_H
