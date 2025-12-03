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
    bool canBet(int amount) { return chips >= amount; };
    int getChips() { return chips; };
    void addChips(int amount) { chips += amount; };
    void subtractChips(int amount) { chips -= amount; };
    int getCurrentBet() { return currentBet; };
    void addToCurrentBet(int amount) { currentBet += amount; };
    bool getFolded() { return folded; };
    void fold() { folded = true; };
    vector<shared_ptr<Card>> heldCards;
    Action makeDecision();
    void resetPlayer();

private:
    int chips = 1000;
    bool isHuman;
    int currentBet = 0;
    bool folded = false;
    queue<Action> decisions;
    // Combined community and personal hand - will reference shared_ptrs
    CardHand fullHand;
    // Shouldn't this be in a view class?
    bool handVisible = false;
};

#endif // PLAYER_H
