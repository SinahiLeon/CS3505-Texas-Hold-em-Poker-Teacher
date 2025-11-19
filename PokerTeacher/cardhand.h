#ifndef CARDHAND_H
#define CARDHAND_H

#include <compare>
#include <vector>
#include <queue>
#include "card.h"
#include "handtype.h"
using std::strong_ordering;
using std::vector;
using std::priority_queue;

class CardHand
{
public:
    CardHand();
    void recieveCards(vector<Card> playerCards);
    Card getHighCard();
    vector<Card> getKickers();
    strong_ordering operator<=>(const CardHand& other) const;
    bool operator==(const Card& other) const;
private:
    vector<Card> cards;
    HandType hand = HandType::Empty;

    HandType evaluateHandType();
    priority_queue<int> getKinds();
    bool hasSecondPair();
    vector<Card> getStraightCards();
    bool hasFlush();
    bool hasFlush(vector<Card>);
};

#endif // CARDHAND_H
