#include "player.h"
#include "QRandomGenerator"

Player::Player() : isHuman(false), fullhand(heldCards) {

}

Player::Player(bool isHuman) : isHuman(isHuman), fullhand(heldCards) {

}

Player::Player(queue<Action> decisions) : isHuman(false), fullhand(heldCards), decisions(decisions) {

}

Player::Player(Player& p) : isHuman(p.isHuman), fullhand(p.fullhand), decisions(p.decisions) {

}

Player Player::operator=(Player& p) {
    if (this == &p){
        return *this;
    }
    isHuman = p.isHuman;
    fullhand = p.fullhand;
    decisions = p.decisions;
    return *this;
}

Action Player::makeDecision() {

    if (isHuman) {
        //If the player is the human player, it needs to receive a signal of what button the user pressed.
        //it will be assumed that the player's buttons are already disabled so the user can't make any illegal moves.

    }


    if(!decisions.empty()) {
        Action decidedAction = decisions.front();
        decisions.pop();
        return decidedAction;
    }

    switch (int choice = QRandomGenerator::global()->bounded(0, 2)) {
        case (0) : { return Check(); }
        case (1) : { return Raise(10); }
        case (2) : { return Fold(); }
        default : { return Fold(); }
    }

    return Fold();
}
