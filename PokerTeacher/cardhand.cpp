#include <algorithm>
#include <QDebug>
#include "cardhand.h"
using std::sort;
using std::nullopt;

CardHand::CardHand(vector<shared_ptr<Card>> startHand) : playerHand(startHand) {
    comparator = [](shared_ptr<Card> l, shared_ptr<Card> r) { return l->value > r->value; };
    sort(playerHand.begin(), playerHand.end(), comparator);
    handType = HandType::HighCard;
}

void CardHand::calculateBestHand(vector<shared_ptr<Card>> communityCards) {
    vector<shared_ptr<Card>> allCards = combineCards(communityCards);

    if (straightFlushCheck(allCards)) {
        if (royalFlushCheck(allCards)) {
            handType = HandType::RoyalFlush;
        }
        else {
            handType = HandType::StraightFlush;
        }
    }
    else if (fourKindCheck(allCards)) {
        handType = HandType::FourKind;
    }
    else if (fullHouseCheck(allCards)) {
        handType = HandType::FullHouse;
    }
    else if (flushCheck(allCards)) {
        handType = HandType::Flush;
    }
    else if (straightCheck(allCards)) {
        handType = HandType::Straight;
    }
    else if (threeKindCheck(allCards)) {
        handType = HandType::ThreeKind;
    }
    else if (twoPairCheck(allCards)) {
        handType = HandType::TwoPair;
    }
    else if (onePairCheck(allCards)) {
        handType = HandType::OnePair;
    }
    else {
        handType = HandType::HighCard;
    }
}

bool CardHand::royalFlushCheck(vector<shared_ptr<Card>>& allCards) {
    return (bestHand[0]->value == CardValue::Ace);
}

bool CardHand::straightFlushCheck(vector<shared_ptr<Card>>& allCards) {
    if (allCards.size() < 5) {
        return false;
    }

    for (int x = 0; x < allCards.size() - 3; x++) {
        if (detectStraight(x, allCards, true) == 5) {
            return true;
        }

    }

    return false;
}

bool CardHand::fourKindCheck(vector<shared_ptr<Card>>& allCards) {
    for (int x = 0; x < allCards.size() - 3; x++) {
        if (*allCards[x] != *allCards[x + 1] || *allCards[x] != *allCards[x + 2] || *allCards[x] != *allCards[x + 3]) {
            continue;
        }

        bestHand = vector<shared_ptr<Card>> {
            allCards[x],
            allCards[x + 1],
            allCards[x + 2],
            allCards[x + 3]
        };

        autoAddKickers(allCards);
        return true;
    }

    return false;
    return false;
}

bool CardHand::fullHouseCheck(vector<shared_ptr<Card>>& allCards) {
    if (allCards.size() < 5) {
        return false;
    }

    bool threePair;
    CardValue threePairValue;
    vector<shared_ptr<Card>> newHand;

    // Three check
    for (int x = 0; x < allCards.size() - 2; x++) {
        if (*allCards[x] == *allCards[x + 1] &&
            *allCards[x] == *allCards[x + 2])
        {
            threePair = true;
            threePairValue = allCards[x]->value;
            newHand.insert(newHand.end(), allCards.begin() + x, allCards.begin() + x + 2);
        }
    }

    if (!threePair) {
        return false;
    }

    // Two check
    for (int x = 0; x < allCards.size() - 1; x++) {
        if (*allCards[x] == *allCards[x + 1] &&
            allCards[x]->value != threePairValue)
        {
            newHand.insert(newHand.end(), allCards.begin() + x, allCards.begin() + x + 1);
            bestHand = vector<shared_ptr<Card>>(std::move(newHand));
            return true;
        }
    }

    return false;
}

bool CardHand::flushCheck(vector<shared_ptr<Card>>& allCards) {
    if (allCards.size() < 5) {
        return false;
    }

    optional<Suit> highestSuit = getHighSuit(allCards);

    if (!highestSuit.has_value()) {
        return false;
    }

    vector<shared_ptr<Card>> newHand;
    for(int x = 0; x < allCards.size() && newHand.size() < 5; x++) {
        if(allCards[x]->suit == *highestSuit) {
            newHand.push_back(allCards[x]);
        }
    }

    bestHand = vector<shared_ptr<Card>>(std::move(newHand));

    return true;
}

optional<Suit> CardHand::getHighSuit(vector<shared_ptr<Card>>& allCards) {
    int hearts = 0;
    int diamonds = 0;
    int clubs = 0;
    int spades = 0;

    for (int x = 0; x < allCards.size(); x++) {
        switch(allCards[x]->suit) {
        case Suit::Heart:
            hearts++;
            break;
        case Suit::Diamond:
            diamonds++;
            break;
        case Suit::Club:
            clubs++;
            break;
        case Suit::Spade:
            spades++;
            break;
        default:
            qDebug() << "suit reading is not correct!";
        }
    }

    if(hearts >= 5) {
        return Suit::Heart;
    }

    else if(diamonds >= 5) {
        return Suit::Diamond;
    }

    else if(clubs >= 5) {
        return Suit::Club;
    }

    else if(spades >= 5) {
        return Suit::Spade;
    }

    return nullopt;
}

bool CardHand::straightCheck(vector<shared_ptr<Card>>& allCards) {
    if (allCards.size() < 5) {
        return false;
    }

    for (int x = 0; x < allCards.size() - 3; x++) {
        if (detectStraight(x, allCards) == 5) {
            return true;
        }
    }

    return false;
}

int CardHand::detectStraight(int start, vector<shared_ptr<Card>>& allCards, bool detectFlush) {
    vector<shared_ptr<Card>> newHand = { allCards[start] };
    int progress = 0;
    Suit startingSuit = allCards[start]->suit;

    for (int x = start; x < allCards.size() - 1; x++) {
        int diff = allCards[x]->value - static_cast<int>(allCards[x + 1]->value);

        if(detectFlush) {
            if (allCards[x]->suit != allCards[x + 1]->suit ||
                startingSuit != allCards[x]->suit)
            {
                continue;
            }
        }


        // Cards are duplicates
        if (diff == 0) {
            continue;
        }

        // Correct straight diff
        if (diff == 1) {
            newHand.push_back(allCards[x + 1]);
            progress++;
        }

        // Diff to big for straight
        if (diff > 1) {
            break;
        }

        // Straight found
        if (progress == 5) {
            bestHand = vector<shared_ptr<Card>>(std::move(newHand));
            break;
        }
    }

    // Low ace straight case
    if (allCards[0]->value == CardValue::Ace &&
        progress == 4 &&
        newHand[newHand.size() - 1]->value == CardValue::Two)
    {
        progress++;
        newHand.push_back(allCards[0]);
        bestHand = vector<shared_ptr<Card>>(std::move(newHand));
    }

    return progress;
}

bool CardHand::threeKindCheck(vector<shared_ptr<Card>>& allCards) {
    for (int x = 0; x < allCards.size() - 2; x++) {
        if (*allCards[x] != *allCards[x + 1] || *allCards[x] != *allCards[x + 2]) {
            continue;
        }

        bestHand = vector<shared_ptr<Card>> {
            allCards[x],
            allCards[x + 1],
            allCards[x + 2]
        };

        autoAddKickers(allCards);
        return true;
    }

    return false;
}

bool CardHand::twoPairCheck(vector<shared_ptr<Card>>& allCards) {
    int pairs = 0;
    vector<shared_ptr<Card>> newHand;

    for (int x = 0; x < allCards.size() - 1; x++) {
        if (*allCards[x] != *allCards[x + 1]) {
            continue;
        }

        newHand.push_back(allCards[x]); // add the pair to the hand
        newHand.push_back(allCards[x + 1]);
        pairs++;

        if (pairs == 2) {
            bestHand = vector<shared_ptr<Card>>(std::move(newHand));
            autoAddKickers(allCards);
            return true;
        }
    }

    return false;
}

bool CardHand::onePairCheck(vector<shared_ptr<Card>>& allCards) {
    for (int x = 0; x < allCards.size() - 1; x++) {
        if (*allCards[x] != *allCards[x + 1]) {
            continue;
        }

        bestHand = vector<shared_ptr<Card>> {
            allCards[x],
            allCards[x + 1]
        };

        autoAddKickers(allCards);
        return true;
    }

    return false;
}

void CardHand::autoAddKickers(vector<shared_ptr<Card>>& allCards) {
    for (int x = 0; x < allCards.size() && bestHand.size() < 6; x++) {
        if (!inBestHand(allCards[x])) { // make sure we're not adding cards already in the best hand
            bestHand.push_back(allCards[x]); // add the highest value kickers
        }
    }
}

vector<shared_ptr<Card>> CardHand::combineCards(vector<shared_ptr<Card>> river) {
    vector<shared_ptr<Card>> allCards = playerHand;
    allCards.insert(allCards.end(), river.begin(), river.end());
    sort(allCards.begin(), allCards.end(), comparator);
    return allCards;
}

bool CardHand::inBestHand(const shared_ptr<Card> card) {
    for (int i = 0; i < bestHand.size(); i++) {
        if (card->exactEqual(*bestHand[i])) {
            return true;
        }
    }
    return false;
}
