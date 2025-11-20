#ifndef CARDHAND_H
#define CARDHAND_H

#include <compare>
#include <vector>
#include <set>
#include <functional>
#include "card.h"
#include "handtype.h"
using std::strong_ordering;
using std::vector;
using std::set;
using std::function;

class CardHand
{
public:
    CardHand(vector<Card*> startHand);
    void recieveCard(Card* card);
    Card* getHighCard();
    strong_ordering operator<=>(const CardHand& other) const;
    bool operator==(const CardHand& other) const;
    HandType decideBestHandType(vector<Card*> river);
private:
    vector<Card*> playerHand;
    vector<Card*> bestHand;
    HandType handType;
    function<bool(Card*, Card*)> comparator;
    const int MIN_RIVER_SIZE = 3;

    void autoAddKickers(vector<Card*>& allCards);
    bool inBestHand(const Card* card);
    bool onePairCheck(vector<Card*>& allCards);
    bool twoPairCheck(vector<Card*>& allCards);
    bool threeKindCheck(vector<Card*>& allCards);
    bool straightCheck(vector<Card*>& allCards);
    bool flushCheck(vector<Card*>& allCards);
    bool fullHouseCheck(vector<Card*>& allCards);
    bool fourKindCheck(vector<Card*>& allCards);
    bool straightFlushCheck(vector<Card*>& allCards);
    bool royalFlushCheck(vector<Card*>& allCards);
    vector<Card*> combineCards(vector<Card*> river);
};

#endif // CARDHAND_H
