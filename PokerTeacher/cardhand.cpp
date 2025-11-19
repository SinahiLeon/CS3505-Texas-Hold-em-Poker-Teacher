#include <unordered_map>
#include "cardhand.h"
using std::unordered_map;

CardHand::CardHand() {}

void CardHand::recieveCards(vector<Card> playerCards) {
    cards = playerCards;
    hand = evaluateHandType();
}

HandType CardHand::evaluateHandType() {
    vector<Card> straightCards = getStraightCards();
    priority_queue<int> kinds = getKinds();

    if (!straightCards.empty()) {
        if (hasFlush(straightCards)) {
            return HandType::StraightFlush;
        }
    }

    if (kinds.top() == 4) {
        return HandType::FourKind;
    }

    if (kinds.top() == 3) {
        if (hasSecondPair()) {
            return HandType::FullHouse;
        }

        return HandType::ThreeKind;
    }

    if (hasFlush()) {
        return HandType::Flush;
    }

    if (!straightCards.empty()) {
        return HandType::Straight;
    }

    if (kinds.top() == 2) {
        if (hasSecondPair()) {
            return HandType::TwoPair;
        }
    }

    return HandType::HighCard;
}

priority_queue<int> CardHand::getKinds() {
    // Unordered map is faster than normal map O(1) instead of O(N)
    unordered_map<CardValue, int> kindPairs;
    priority_queue<int> kindQueue;

    // Build map
    for (const auto& card : cards) {
        kindPairs[card.value]++;
    }

    // Add map items into priority queue, auto sorting them
    for (const auto& pair : kindPairs) {
        kindQueue.push(pair.second);
    }

    return kindQueue;
}


