#include <algorithm>
#include "cardhand.h"
using std::sort;

CardHand::CardHand(vector<Card*> startHand) : playerHand(startHand) {
    comparator = [](Card* l, Card* r) { return l->value > r->value; };
    sort(playerHand.begin(), playerHand.end(), comparator);
    handType = HandType::HighCard;
}

HandType CardHand::decideBestHandType(vector<Card*> river) {
    vector<Card*> allCards = combineCards(river);

    if (straightFlushCheck(allCards)) {
        if (royalFlushCheck(allCards)) {
            return HandType::RoyalFlush;
        }

        return HandType::StraightFlush;
    }
    else if (fourKindCheck(allCards)) {
        return HandType::FourKind;
    }
    else if (fullHouseCheck(allCards)) {
        return HandType::FullHouse;
    }
    else if (flushCheck(allCards)) {
        return HandType::Flush;
    }
    else if (straightCheck(allCards)) {
        return HandType::Straight;
    }
    else if (threeKindCheck(allCards)) {
        return HandType::ThreeKind;
    }
    else if (twoPairCheck(allCards)) {
        return HandType::TwoPair;
    }
    else if (onePairCheck(allCards)) {
        return HandType::OnePair;
    }
    else {
        return HandType::HighCard;
    }
}

bool CardHand::royalFlushCheck(vector<Card*>& allCards) {
    // TODO: Implement this
    return false;
}

bool CardHand::straightFlushCheck(vector<Card*>& allCards) {
    // TODO: Implement this
    return false;
}

bool CardHand::fourKindCheck(vector<Card*>& allCards) {
    // TODO: Implement this
    return false;
}

bool CardHand::fullHouseCheck(vector<Card*>& allCards) {
    // TODO: Implement this
    return false;
}

bool CardHand::flushCheck(vector<Card*>& allCards) {
    // TODO: Implement this
    return false;
}

bool CardHand::straightCheck(vector<Card*>& allCards) {
    // Edge cases:
    // Ace case
    // Sift through duplicate cards to find straight

    // On each step:
    // if (curr == last) no straight progress
    // if (curr == last - 1) straight progress
    // if (curr <= last - 2) no straight

    // On each start:
    // prog needed is five
    // if (first == ace) ace exists
    // if (curr == 5) check for low ace
        // prog needed is four
    // if (curr index == 4) no straight possible

    return false;
}

bool CardHand::threeKindCheck(vector<Card*>& allCards) {
    for (int x = 0; x < allCards.size() - 2; x++) {
        if (*allCards[x] != *allCards[x + 1] || *allCards[x] != *allCards[x + 2]) {
            continue;
        }

        bestHand.push_back(allCards[x]); // add the pair to the hand
        bestHand.push_back(allCards[x + 1]);

        autoAddKickers(allCards);
        return true;
    }

    return false;
}

bool CardHand::twoPairCheck(vector<Card*>& allCards) {
    int pairs = 0;
    vector<Card*> newHand;

    for (int x = 0; x < allCards.size() - 1; x++) {
        if (*allCards[x] != *allCards[x + 1]) {
            continue;
        }

        newHand.push_back(allCards[x]); // add the pair to the hand
        newHand.push_back(allCards[x + 1]);
        pairs++;

        if (pairs == 2) {
            bestHand = vector<Card*>(std::move(newHand));
            autoAddKickers(allCards);
            return true;
        }
    }

    return false;
}

bool CardHand::onePairCheck(vector<Card*>& allCards) {
    vector<Card*> newHand;

    for (int x = 0; x < allCards.size() - 1; x++) {
        if (*allCards[x] != *allCards[x + 1]) {
            continue;
        }

        newHand.push_back(allCards[x]); // add the pair to the hand
        newHand.push_back(allCards[x + 1]);
        bestHand = vector<Card*>(std::move(newHand));

        autoAddKickers(allCards);
        return true;
    }

    return false;
}

void CardHand::autoAddKickers(vector<Card*>& allCards) {
    for (int x = 0; x < allCards.size() && bestHand.size() < 6; x++) {
        if (!inBestHand(allCards[x])) { // make sure we're not adding cards already in the best hand
            bestHand.push_back(allCards[x]); // add the highest value kickers
        }
    }
}

vector<Card*> CardHand::combineCards(vector<Card*> river) {
    vector<Card*> allCards = playerHand;
    allCards.insert(allCards.end(), river.begin(), river.end());
    sort(allCards.begin(), allCards.end(), comparator);
    return allCards;
}

bool CardHand::inBestHand(const Card* card) {
    for (int i = 0; i < bestHand.size(); i++) {
        if (card->exactEqual(*bestHand[i])) {
            return true;
        }
    }
    return false;
}
