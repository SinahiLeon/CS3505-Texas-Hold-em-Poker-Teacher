#include "player.h"
#include "QRandomGenerator"

using std::min;

Player::Player() : fullHand(heldCards) {

}

Player::Player(queue<Action> decisions) : fullHand(heldCards), decisions(decisions) {

}

Player::Player(const Player& p)
    : chips(p.chips)
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

void Player::giveNewActions(queue<Action> actions) {
    while (!actions.empty()) {
        decisions.push(actions.front());
        actions.pop();
    }
}
