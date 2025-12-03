#include "player.h"
#include "QRandomGenerator"

Player::Player() : isHuman(false), fullhand(heldCards) {

}

Player::Player(bool isHuman) : isHuman(isHuman), fullhand(heldCards) {

}

Action Player::makeDecision() {
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
