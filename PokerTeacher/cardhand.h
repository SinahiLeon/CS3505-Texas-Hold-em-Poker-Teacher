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
    /// @brief Constructs a new CardHand.
    /// @param startHand The player's starting hand.
    CardHand(vector<shared_ptr<Card>> startHand);
    /// @brief Returns the best card in the deck, either the highest non-kicker card or the highest card in general.
    shared_ptr<Card> getBestCard() const;
    /// @brief Compares two CardHands and returns whether they are greater, equal to, or less than each other according to poker rules.
    /// @remarks This operator inherently generates the <, >, <=, >=, and != operators.
    /// @returns Strong ordering of two card hands, which can be implicitly turned into a bool.
    strong_ordering operator<=>(const CardHand& other) const;
    /// @brief Compares two CardHands and returns whether they are equal.
    bool operator==(const CardHand& other) const;
    /// @brief Calculates the best hand using the set of player cards and community cards.
    /// @param communityCards Cards owned by the table, also known as the river cards.
    void calculateBestHand(vector<shared_ptr<Card>> communityCards);
    HandType getHandType() const;
private:
    vector<shared_ptr<Card>> playerHand;
    vector<shared_ptr<Card>> bestHand;
    HandType handType;
    /// @brief Comparator for sorting dereferenced card pointers.
    function<bool(shared_ptr<Card>, shared_ptr<Card>)> comparator;

    /// @brief Ranks kickers sequentially.
    /// @param other Card to compare kickers to.
    /// @param start Start of the kickers in the hand.
    /// @returns A strong ordering of which hand has stronger kickers.
    strong_ordering rankKickers(const CardHand& other, int start) const;
    /// @brief Adds the highest community cards to a hand to fill out kickers.
    /// @param allCards A vector of all community cards plus the player's hand.
    void autoAddKickers(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a card already exists in the best hand.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool inBestHand(const shared_ptr<Card> card) const;
    /// @brief Combines the player's hand with the community cards.
    /// @param river The community cards owned by the table.
    /// @returns A sorted vector of the player's hand and community cards.
    vector<shared_ptr<Card>> combineCards(vector<shared_ptr<Card>> communityCards) const;
    /// @brief Returns if a hand has one pair.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool onePairCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a hand has two pairs.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool twoPairCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a hand has three identical cards.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool threeKindCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a hand has a straight.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool straightCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Finds the progress a hand has towards a straight.
    /// @param allCards A vector of all community cards plus the player's hand.
    /// @returns How many cards are in a straight, not counting low aces.
    int detectStraight(int curr, vector<shared_ptr<Card>>& allCards, bool detectFlush = false);
    /// @brief Returns if a hand has a flush.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool flushCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns either the flush suit of a hand, or a nullopt if the hand does not have a flush.
    /// @param allCards A vector of all community cards plus the player's hand.
    optional<Suit> getFlushSuit(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a hand has a full house.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool fullHouseCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a hand has a four of a kind.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool fourKindCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a hand has a straight flush.
    /// @param allCards A vector of all community cards plus the player's hand.
    bool straightFlushCheck(vector<shared_ptr<Card>>& allCards);
    /// @brief Returns if a straight flush is a royal flush.
    bool royalFlushCheck();
};

#endif // CARDHAND_H
