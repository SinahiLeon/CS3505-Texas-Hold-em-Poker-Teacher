#ifndef PLAYER_H
#define PLAYER_H

#include "cardhand.h"

/// @brief A player that containts their information on chips, currentBet, folded, and hand data.
class Player {
public:
    Player() : fullhand(heldCards) {}
    bool canBet(int amount) { return chips >= amount; }
    int chips = 50;
    int currentBet = 0;
    bool folded = false;
    bool handVisible = false;
    vector<shared_ptr<Card>> heldCards;

    // Combined community and personal hand - will reference shared_ptrs
    CardHand fullhand;
};

#endif // PLAYER_H
