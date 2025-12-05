#include "player.h"
#include "QRandomGenerator"

using std::min;

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
    heldCards = p.heldCards;
    fullHand = p.fullHand;
    decisions = p.decisions;
    return *this;
}

void Player::resetPlayer() {
    bet = 0;
    folded = false;
}

int Player::makeBet(int amount) {
    int possibleBet = min(amount, chips);
    bet += possibleBet;
    chips -= possibleBet;
    return possibleBet;
}

Action Player::makeDecision(int currentBet, Action playerAction) {
    if (allIn() || folded || busted()) {
        return Action::None;
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
        switch (int choice = QRandomGenerator::global()->bounded(2)) {
            case (0) : { maintainBet(currentBet); }
            case (1) : {
                folded = true;
                return Action::Fold;
            }
            default : {
                folded = true;
                return Action::Fold;
            }
        }
    }

    switch (int choice = QRandomGenerator::global()->bounded(3)) {
        case (0) : { return maintainBet(currentBet); }
        // I don't think there should be a distinction between betting and raising in the code.
        case (1) : {
            int amount = 10;

            if (amount > chips) {
                maintainBet(currentBet);
            }

            return Action::Raise;
        }
        case (2) : {
            folded = true;
            return Action::Fold;
        }
        default : {
            folded = true;
            return Action::Fold;
        }
    }
}

Action Player::maintainBet(int currentBet) {
    if (currentBet == bet) {
        return Action::Check;
    }

    bet = currentBet;
    return Action::Call;
}
