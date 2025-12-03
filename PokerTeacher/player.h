#ifndef PLAYER_H
#define PLAYER_H

#include "cardhand.h"
#include "action.h"
#include <qobject.h>
#include <queue>

using std::queue;

/// @brief A player that containts their information on chips, currentBet, folded, and hand data.
class Player : public QObject {
public:
    Player();
    Player(bool isHuman);
    Player(queue<Action> decisions);
    Player(const Player& other);
    Player operator=(const Player& other);
    bool isHuman;
    bool canBet(int amount) { return chips >= amount; }
    int chips = 1000;
    int currentBet = 0;
    bool folded = false;
    bool handVisible = false;
    vector<shared_ptr<Card>> heldCards;
    queue<Action> decisions;

    // Combined community and personal hand - will reference shared_ptrs
    CardHand fullHand;

    Action makeDecision();

public slots:


};

#endif // PLAYER_H
