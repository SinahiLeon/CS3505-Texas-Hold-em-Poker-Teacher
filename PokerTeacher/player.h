#ifndef PLAYER_H
#define PLAYER_H

#include "cardhand.h"
#include "action.h"
#include <qobject.h>
#include <queue>

using std::queue;
using std::monostate;

/// @brief A player that containts their information on chips, currentBet, folded, and hand data.
class Player {
public:
    Player();
    Player(queue<Action> decisions);
    Player(const Player& other);
    Player operator=(const Player& other);
    bool canBet(int amount) { return chips >= amount; };
    int getChips() { return chips; };
    void addChips(int amount) { chips += amount; };
    /// @brief subracts the chips from player, and adds them to their bet, the bet amount is then returned.
    /// If the bet is too large, it returns the largest amount the player could be without going into the negative.
    /// @param amount the amount the player would bet.
    /// @return the amount of chips subtracted from the player.
    int makeBet(int amount);
    int getBet() { return bet; };
    void addToBet(int amount) { bet += amount; };
    bool getFolded() { return folded; };
    /// @brief Determines whether the player is in the game and can win the pot.
    /// @return if the player can win the pot.
    bool canWinPot() {return !folded && !busted(); }
    void fold() { folded = true; };
    vector<shared_ptr<Card>> heldCards;
    Action makeDecision(int currentBet, Action playerAction = Action::None);
    int decideHandValue(bool canRaise);
    void resetPlayer();
    int getRaiseAmount();
    void giveNewActions(queue<Action> actions);

private:
    int chips = 1000;
    int bet = 0;
    bool folded = false;
    queue<Action> decisions;
    // Combined community and personal hand - will reference shared_ptrs
    CardHand fullHand;

    Action maintainBet(int currentBet);

    /// @brief determines if the player is all in, they can win the pot, but can't bet anymore because they have no money.
    /// @return if the player is all in.
    bool allIn() { return chips == 0 && bet > 0; };

    /// @brief determines if the player is busted, they have no money and have none in the pot. They are essentally out of the game.
    /// @return if the player is busted.
    bool busted() { return chips == 0 && bet == 0; }
};

#endif // PLAYER_H
