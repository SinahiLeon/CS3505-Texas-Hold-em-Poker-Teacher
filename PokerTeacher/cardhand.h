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
using std::pair;

class CardHand
{
public:
    CardHand(vector<shared_ptr<Card>> startHand);
    /// @brief Returns the best card in the deck, either the highest non-kicker card or the highest card in general.
    shared_ptr<Card> getBestCard() const;
    strong_ordering operator<=>(const CardHand& other) const;
    bool operator==(const CardHand& other) const;
    void calculateBestHand(vector<shared_ptr<Card>> communityCards);
    HandType getHandType() const;
private:
    vector<shared_ptr<Card>> playerHand;
    vector<shared_ptr<Card>> bestHand;
    HandType handType;
    function<bool(shared_ptr<Card>, shared_ptr<Card>)> comparator;
    const int MIN_RIVER_SIZE = 3;

    strong_ordering rankKickers(const CardHand& other, int start) const;
    void autoAddKickers(vector<shared_ptr<Card>>& allCards);
    bool inBestHand(const shared_ptr<Card> card) const;
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
    vector<shared_ptr<Card>> combineCards(vector<shared_ptr<Card>> river) const;
};

#endif // CARDHAND_H
