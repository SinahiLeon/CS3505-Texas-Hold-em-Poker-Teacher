#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "cardhand.h"
#include <QObject>
#include <stack>
#include <QMap>
using std::stack;

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
        bool handVisible = false;
        vector<shared_ptr<Card>> heldCards;

        // Combined community and personal hand - will reference shared_ptrs
        CardHand fullhand;
    };

    /// @brief Used to return which player we are on.
    /// @param int Index within the players vector.
    /// @return address of Player.
    Player& getPlayer(int index) { return players[index]; }
    int getPot() const { return pot; }
    int getCurrentBet() const { return currentBet; }
    int getBigBlind() const { return bigBlind; }
    int getSmallBlind() const { return smallBlind; }
    const vector<shared_ptr<Card>>& getCommunityCards() const { return communityCards; }

    void makeBet(int playerIndex, int chipAmount);
    void call(int playerIndex);
    void raise(int playerIndex, int chipAmount);
    void fold(int playerIndex);
    Phases getPhase() const { return phase; }

    void newGame();
    void nextPhase();

signals:    // Since a value changed, signal to update UI
    void chipsUpdated(int playerIndex, int newAmount);
    void potUpdated(int newAmount);
    void communityCardsUpdated();
    void phaseUpdated(Phases currPhase);
    void playerFolded(int playerIndex);
    void currentBetUpdated(int newBet);

private:
    stack<shared_ptr<Card>> deck;
    vector<shared_ptr<Card>> communityCards;
    vector<Player> players;

    int pot = 0;
    int currentBet = 0;
    int bigBlind = 10;
    int smallBlind = 5;
    int phaseIndex = 0;
    Phases phase = Phases::Preflop;

    QMap<int, Phases> phaseIndices{
        {0, Phases::Preflop},
        {1, Phases::Flop},
        {2, Phases::Turn},
        {3, Phases::River},
        {4, Phases::Showdown}};


    /// @brief Resets and shuffles the deck with all cards
    void shuffleDeck();
    /// @brief Helper method to empty the deck stack
    void clearDeck();
    /// @brief Gives pot to the winner (used inside fold or showdown)
    void awardPotToPlayer(int playerIndex); // why are there two of these?
    /// @brief
    void dealCards(int cardAmount, vector<shared_ptr<Card>>& target);
    /// @brief
    void dealHoleCards();

};

#endif // GAME_H
