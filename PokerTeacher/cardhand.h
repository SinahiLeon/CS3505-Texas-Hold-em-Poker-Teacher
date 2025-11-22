#ifndef CARDHAND_H
#define CARDHAND_H

#include <compare>
#include <vector>
#include <set>
#include <functional>
#include <optional>
#include "card.h"
#include "handtype.h"
using std::strong_ordering;
using std::vector;
using std::set;
using std::function;
using std::optional;
using std::shared_ptr;

class CardHand
{
public:
    CardHand(vector<shared_ptr<Card>> startHand);
    void recieveCard(shared_ptr<Card> card);
    shared_ptr<Card> getHighCard();
    strong_ordering operator<=>(const CardHand& other) const;
    bool operator==(const CardHand& other) const;
    void calculateBestHand(vector<shared_ptr<Card>> communityCards);
    HandType getHandType();
private:
    vector<shared_ptr<Card>> playerHand;
    vector<shared_ptr<Card>> bestHand;
    HandType handType;
    function<bool(shared_ptr<Card>, shared_ptr<Card>)> comparator;
    const int MIN_RIVER_SIZE = 3;

    void autoAddKickers(vector<shared_ptr<Card>>& allCards);
    bool inBestHand(const shared_ptr<Card> card);
    bool onePairCheck(vector<shared_ptr<Card>>& allCards);
    bool twoPairCheck(vector<shared_ptr<Card>>& allCards);
    bool threeKindCheck(vector<shared_ptr<Card>>& allCards);
    bool straightCheck(vector<shared_ptr<Card>>& allCards);
    int detectStraight(int curr, vector<shared_ptr<Card>>& allCards, bool detectFlush = false);
    bool flushCheck(vector<shared_ptr<Card>>& allCards);
    optional<Suit> getHighSuit(vector<shared_ptr<Card>>& allCards);
    bool fullHouseCheck(vector<shared_ptr<Card>>& allCards);
    bool fourKindCheck(vector<shared_ptr<Card>>& allCards);
    bool straightFlushCheck(vector<shared_ptr<Card>>& allCards);
    int detectStraightFlush(int start, vector<shared_ptr<Card>>& allCards);
    bool royalFlushCheck(vector<shared_ptr<Card>>& allCards);
    vector<shared_ptr<Card>> combineCards(vector<shared_ptr<Card>> river);
};

#endif // CARDHAND_H
