#include <algorithm>
#include "cardhand.h"
using std::sort;
using std::greater;

CardHand::CardHand(vector<Card> startHand) : playerHand(startHand) {
    sort(playerHand.begin(), playerHand.end(), greater<Card>());
    handType = HandType::HighCard;
}

void CardHand::recieveCards(vector<Card> riverCards) {
    allCards = playerHand;
    allCards.insert(allCards.end(), riverCards.begin(), riverCards.end());
    sort(allCards.begin(), allCards.end());
    handType = decideBestHandType();
}

HandType CardHand::decideBestHandType() {
    if (straightFlushCheck()) {
        if (royalFlushCheck()) {
            return HandType::RoyalFlush;
        }

        return HandType::StraightFlush;
    }
    else if (fourKindCheck()) {
        return HandType::FourKind;
    }
    else if (fullHouseCheck()) {
        return HandType::FullHouse;
    }
    else if (flushCheck()) {
        return HandType::Flush;
    }
    else if (straightCheck()) {
        return HandType::Straight;
    }
    else if (threeKindCheck()) {
        return HandType::ThreeKind;
    }
    else if (twoPairCheck()) {
        return HandType::TwoPair;
    }
    else if (onePairCheck()) {
        return HandType::OnePair;
    }
    else {
        return HandType::HighCard;
    }
}

bool CardHand::royalFlushCheck() {
    // TODO: Implement this
    return false;
}

bool CardHand::straightFlushCheck() {
    // TODO: Implement this
    return false;
}

bool CardHand::fourKindCheck() {
    // TODO: Implement this
    return false;
}

bool CardHand::fullHouseCheck() {
    // TODO: Implement this
    return false;
}

bool CardHand::flushCheck() {
    // TODO: Implement this
    return false;
}

bool CardHand::straightCheck() {
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

bool CardHand::threeKindCheck() {
    for (int x = 0; x < allCards.size() - 2; x++) {
        if (allCards[x] != allCards[x + 1] || allCards[x] != allCards[x + 2]) {
            continue;
        }

        bestHand.push_back(allCards[x]); // add the pair to the hand
        bestHand.push_back(allCards[x + 1]);

        autoAddKickers();
        return true;
    }

    return false;
}

bool CardHand::twoPairCheck() {
    int pairs = 0;
    vector<Card> newHand;

    for (int x = 0; x < allCards.size() - 1; x++) {
        if (allCards[x] != allCards[x + 1]) {
            continue;
        }

        newHand.push_back(allCards[x]); // add the pair to the hand
        newHand.push_back(allCards[x + 1]);
        pairs++;

        if (pairs == 2) {
            bestHand = vector<Card>(std::move(newHand));
            autoAddKickers();
            return true;
        }
    }

    return false;
}

bool CardHand::onePairCheck() {
    vector<Card> newHand;

    for (int x = 0; x < allCards.size() - 1; x++) {
        if (allCards[x] != allCards[x + 1]) {
            continue;
        }

        newHand.push_back(allCards[x]); // add the pair to the hand
        newHand.push_back(allCards[x + 1]);
        bestHand = vector<Card>(std::move(newHand));

        autoAddKickers();
        return true;
    }

    return false;
}

void CardHand::autoAddKickers() {
    for (int x = 0; x < allCards.size() && bestHand.size() < 6; x++) {
        if (!inBestHand(allCards[x])) { // make sure we're not adding cards already in the best hand
            bestHand.push_back(allCards[x]); // add the highest value kickers
        }
    }
}

bool CardHand::inBestHand(const Card& card) {
    for (int i = 0; i < bestHand.size(); i++) {
        if(card.exactEqual(bestHand[i])) {
            return true;
        }
    }
    return false;
}
