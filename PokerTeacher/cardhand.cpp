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
    bestHand = getBestHand();
}

vector<Card> CardHand::getBestHand() {

}

bool CardHand::onePairCheck() {
    int start = 0;
    int end = 4;

    for (int x = 0; x < allCards.size() - 1; x++) {
        if (allCards[x] == allCards[x + 1]) {
            return true;
        }
    }
}
