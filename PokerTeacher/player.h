#ifndef PLAYER_H
#define PLAYER_H

#include "cardhand.h"
#include "action.h"
#include <queue>

using std::queue;

/// @brief A player that containts their information on chips, currentBet, folded, and hand data.
class Player {
public:
    Player();
    Player(bool isHuman);
    Player(queue<Action> decisions);
    bool const isHuman;
    bool canBet(int amount) { return chips >= amount; }
    int chips = 1000;
    int currentBet = 0;
    bool folded = false;
    bool handVisible = false;
    vector<shared_ptr<Card>> heldCards;
    queue<Action> decisions;

    // Combined community and personal hand - will reference shared_ptrs
    CardHand fullhand;

    Action makeDecision();

};

#endif // PLAYER_H
