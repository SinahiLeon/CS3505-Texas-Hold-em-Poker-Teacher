#ifndef CARDHAND_H
#define CARDHAND_H

#include <compare>
#include <vector>
#include <set>
#include "card.h"
#include "handtype.h"
using std::strong_ordering;
using std::vector;
using std::set;

class CardHand
{
public:
    CardHand(vector<Card> startHand);
    void recieveCards(vector<Card> riverCards);
    Card getHighCard();
    vector<Card> getKickers();
    strong_ordering operator<=>(const CardHand& other) const;
    bool operator==(const Card& other) const;
private:
    vector<Card> playerHand;
    vector<Card> allCards;
    vector<Card> bestHand;
    HandType handType;
    const int MIN_RIVER_SIZE = 3;

    HandType decideBestHandType();
    void autoAddKickers();
    bool inBestHand(const Card& card);
    bool onePairCheck();
    bool twoPairCheck();
    bool threeKindCheck();
    bool straightCheck();
    bool flushCheck();
    bool fullHouseCheck();
    bool fourKindCheck();
    bool straightFlushCheck();
    bool royalFlushCheck();
};

#endif // CARDHAND_H
