#include <algorithm>
#include "cardhand.h"
using std::sort;
using std::greater;

CardHand::CardHand(vector<Card> startHand) : playerHand(startHand) {
    sort(playerHand.begin(), playerHand.end(), greater<Card>());
    hand = HandType::HighCard;
}

void CardHand::recieveCards(vector<Card> riverCards) {
    allCards = playerHand;
    allCards.insert(allCards.end(), riverCards.begin(), riverCards.end());
    sort(allCards.begin(), allCards.end());
    decideBestHand();
}

void CardHand::decideBestHand() {

    if(onePairCheck()) {
        hand = HandType::OnePair;
    }

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

bool CardHand::onePairCheck() {
    for (int x = 0; x < allCards.size() - 1; x++) {
        if (allCards[x] == allCards[x + 1]) {
            bestHand.push_back(allCards[x]); // add the pair to the hand
            bestHand.push_back(allCards[x + 1]);

            autoAddKickers();
            return true;
        }
    }
    return false;
}
