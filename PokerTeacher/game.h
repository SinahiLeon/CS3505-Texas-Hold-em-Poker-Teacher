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

    /// @brief Different phases of Texas Hold'em poker
    enum class Phases {
        Preflop,
        Flop,   // Three community cards are dealt in the middle of the table.
        Turn ,   // Fourth card card dealt.
        River,  // The fifth and final community card is dealt.
        Showdown,   // If more than one player remains after the last betting round, players
                    // reveal their hands. The player with the best five-card hand wins the pot.
    };

    /// @brief A player that containts their information on chips, currentBet, folded, and hand data.
    class Player {
    public:
        int chips = 0;
        int currentBet = 0;
        bool folded = false;
        std::vector<Card*> hand;
    };

    /// @brief Used to return which player we are on.
    /// @param int Index within the players vector.
    /// @return address of Player.
    Player& getPlayer(int index) { return players[index]; }
    int getPot() const { return pot; }
    void makeBet(int playerIndex, int chipAmount);
    Phases getPhase() const { return phase; }

signals:    // Since a value changed, signal to update UI
    void chipsUpdated(int playerIndex, int newAmount);
    void potUpdated(int newAmount);
    //phaseUpdated

private:
    std::stack<const std::pair<const QString, Card>*> deck;
    std::vector<const std::pair<const QString, Card>*> communityCards;
    int pot;
    int currentBet;
    int bigBlind;
    int smallBlind;
    int phaseIndex;
    Phases phase;
    std::vector<Player> players;
    QMap<int, Phases> phaseIndices{{0, Phases::Preflop}, {1, Phases::Flop}, {2, Phases::Turn}, {3, Phases::River}, {4, Phases::Showdown}};

    /// @brief
    void newGame();
    /// @brief Resets and shuffles the deck with all cards
    void shuffleDeck();
    /// @brief Helper method to empty the deck stack
    void clearDeck();
    /// @brief Gives pot to the winner (used inside fold or showdown)
    void awardPotToPlayer(bool playerWins);
    void awardPotToPlayer(int playerIndex); // why are there two of these?
    /// @brief
    void nextPhase();
};

#endif // GAME_H
