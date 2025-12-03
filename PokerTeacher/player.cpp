#include "player.h"
#include "QRandomGenerator"

Player::Player() :isHuman(false), fullHand(heldCards) {

}

Player::Player(bool isHuman) : isHuman(isHuman), fullHand(heldCards) {

}

Player::Player(queue<Action> decisions) : isHuman(false), fullHand(heldCards), decisions(decisions) {

}

Player::Player(const Player& p)
    : isHuman(p.isHuman)
    , chips(p.chips)
    , bet(p.bet)
    , folded(p.folded)
    , handVisible(p.handVisible)
    , heldCards(p.heldCards)
    , fullHand(p.fullHand)
    , decisions(p.decisions)
{

}

Player Player::operator=(const Player& p) {
    if (this == &p){
        return *this;
    }
    isHuman = p.isHuman;
    chips = p.chips;
    bet = p.bet;
    folded = p.folded;
    handVisible = p.handVisible;
    heldCards = p.heldCards;
    fullHand = p.fullHand;
    decisions = p.decisions;
    return *this;
}

Action Player::makeDecision(int currentBet, Action playerAction) {
    if (allIn()) {
        return None();
    }

    if (isHuman) {
        //If the player is the human player, it needs to receive a signal of what button the user pressed.
        //it will be assumed that the player's buttons are already disabled so the user can't make any illegal moves.

    }

    if (!decisions.empty()) {
        Action decidedAction = decisions.front();
        decisions.pop();
        return decidedAction;
    }

    if (currentBet > chips) {
        switch (int choice = QRandomGenerator::global()->bounded(0, 2)) {
            case (0) : { maintainBet(currentBet); }
            // I don't think there should be a distinction between betting and raising in the code.
            case (1) : { return Fold(); }
        default : { return Fold(); }
        }
    }

    switch (int choice = QRandomGenerator::global()->bounded(0, 3)) {
        case (0) : { maintainBet(currentBet); }
        // I don't think there should be a distinction between betting and raising in the code.
        case (1) : {
            int amount = getRaiseAmount();

            if (amount > chips) {
                maintainBet(currentBet);
            }

            bet = currentBet + amount;
            return Raise(10);
        }
        case (2) : { return Fold(); }
        default : { return Fold(); }
    }

    return Fold();
}

int Player::getRaiseAmount() {
    // At some point there will be fancy AI logic here
    return 10;
}

Action Player::maintainBet(int currentBet) {
    if (currentBet == bet) {
        return Check();
    }

    bet = currentBet;
    return Call();
}
