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

Action Player::makeDecision(int currentBet, Action playerAction) {
    if (allIn() || folded) {
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

    switch (decideHandValue(currentBet <= chips)) {
        case (0) : { return maintainBet(currentBet); }
        // I don't think there should be a distinction between betting and raising in the code.
        case (1) : {
            folded = true;
            return Action::Fold;
        }
        case (2) : {
            int amount = 10;

            if (amount > chips) {
                maintainBet(currentBet);
            }

            return Action::Raise;
        }
        default : {
            folded = true;
            return Action::Fold;
        }
    }
}

int Player::decideHandValue(bool canRaise) {
    if(canRaise) {
        return  QRandomGenerator::global()->bounded(3);
    } else {
        return QRandomGenerator::global()->bounded(2);
    }
}

Action Player::maintainBet(int currentBet) {
    if (currentBet == bet) {
        return Action::Check;
    }

    bet = currentBet;
    return Action::Call;
}
