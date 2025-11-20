#ifndef GAME_H
#define GAME_H

#include "card.h"
#include <QObject>
#include <stack>
#include <vector>
#include <Qmap>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);

    // Different phases of Texas Hold'em poker
    enum class Phases {
        Preflop,
        Flop,   // Three community cards are dealt in the middle of the table.
        Turn ,   // Fourth card card dealt.
        River,  // The fifth and final community card is dealt.
        Showdown,   // If more than one player remains after the last betting round, players
                    // reveal their hands. The player with the best five-card hand wins the pot.
    };


    class Player {
    public:
        int chips = 0;
        int currentBet = 0;
        bool folded = false;
        std::vector<Card*> hand;
    };

    void newGame();

    Player& getPlayer(int index) { return players[index]; }
    int getPot() const { return pot; }
    void makeBet(int playerIndex, int chipAmount);
    Phases getPhase() const { return phase; }
    void awardPotToPlayer(int playerIndex);
    void nextPhase();


signals:    // Since a value changed, signal to update UI
    void chipsUpdated(int playerIndex, int newAmount);
    void potUpdated(int newAmount);
    //phaseUpdated

private:
    // Cards
    std::stack<Card*> deck; // 52 card deck
    std::vector<Card*> river;   // Community cards
    std::vector<Player> players;

    int pot;
    int currentBet;
    int bigBlind;
    int smallBlind;
    int phaseIndex;
    QMap<int, Phases> phaseIndices{{0, Phases::Preflop}, {1, Phases::Flop}, {2, Phases::Turn}, {3, Phases::River}, {4, Phases::Showdown}};

    Phases phase;
};

#endif // GAME_H
