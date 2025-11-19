#ifndef GAME_H
#define GAME_H

#include "card.h"
#include <QObject>
#include <stack>
#include <vector>


class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);

    // Different phases of Texas Hold'em poker
    enum class Phases {
        Preflop,
        Flop,   // Three community cards are dealt face up in the middle of the table
        River,  // The fifth and final community card is dealt face up
        Showdown,   // If more than one player remains after the last betting round, players
                    // reveal their hands. The player with the best five-card hand wins the pot.
    };






signals:

private:
    // Cards
    std::stack<Card*> deck;
    std::vector<Card*> river;   // Community cards
    std::vector<Card*> playerHand;  // 2 cards
    std::vector<Card*> computerHand;    // 2 cards

    // Betting state
    int playerChips;
    int computerChips;
    int pot;
    int currentBet;    // amount that must be called in this round
    int smallBlind;
    int bigBlind;

    Phases phase;
};

#endif // GAME_H
