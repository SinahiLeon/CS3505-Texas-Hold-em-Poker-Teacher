#ifndef PLAYER_H
#define PLAYER_H

#include "cardhand.h"
#include "action.h"
#include <qobject.h>
#include <queue>

using std::queue;
using std::monostate;

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
    int getBet() { return bet; };
    void addToBet(int amount) { bet += amount; };
    bool getFolded() { return folded; };
    void fold() { folded = true; };
    vector<shared_ptr<Card>> heldCards;
    Action makeDecision(int currentBet, Action playerAction = Action::None);
    void resetPlayer();
    int getRaiseAmount();

private:
    int chips = 1000;
    bool isHuman;
    int bet = 0;
    bool folded = false;
    queue<Action> decisions;
    // Combined community and personal hand - will reference shared_ptrs
    CardHand fullHand;

    Action maintainBet(int currentBet);
    bool allIn() { return chips == 0 && bet > 0; };
};

#endif // PLAYER_H
