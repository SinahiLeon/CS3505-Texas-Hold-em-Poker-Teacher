#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "cardhand.h"
#include <QObject>
#include <stack>
#include <QMap>
#include "phase.h"
#include "player.h"

using std::stack;

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);

    /// @brief Used to return which player we are on.
    /// @param int Index within the players vector.
    /// @return address of Player.
    Player& getPlayer(int index) { return players[index]; }
    vector<shared_ptr<Card>> communityCards;
    vector<Player> players;
    int getDealerIndex() { return dealerIndex; }
    int getPot() const { return pot; }
    int getBetAmount() const { return currentBet; }
    int getRaiseAmount() const { return getBetAmount() + getLargeBlind(); }
    int getLargeBlind() const { return bigBlind; }
    int getSmallBlind() const { return smallBlind; }
    const vector<shared_ptr<Card>>& getCommunityCards() const { return communityCards; }

    void check(int playerIndex);
    void makeBet(int playerIndex, int chipAmount, bool isCall);
    void call(int playerIndex);
    void raise(int playerIndex, int chipAmount);
    void fold(int playerIndex);
    /// @brief Compares players best hands to determine winner.
    /// @return int of the player who will be awarded the pot.
    int determineIndexOfWinner();
    Phase getPhase() const { return phase; }
    bool noBetsYetThisPhase;

signals:    // Since a value changed, signal to update UI
    void chipsUpdated(int playerIndex, int chips, int bet);
    void dealerUpdate(int playerIndex);
    void potUpdated(int newAmount);
    void communityCardsUpdated();
    void handCardsUpdated();
    void phaseUpdated(Phase currPhase);
    void playerFolded(int playerIndex);
    void currentBetUpdated(int newBet);
    void updateLastAction(int playerIndex, QString action);
    void updateAvailableActions();
    void handEnded(int winner);

public slots:
    void onViewInitialized();
    void playerMakesBet(int amount);
    void playerCalls();
    void playerChecks();
    void playerFolds();
    void startNewGame();

private:
    stack<shared_ptr<Card>> deck;
    int dealerIndex = 2;
    int pot = 0;
    int currentBet = 0;
    int bigBlind = 10;
    int smallBlind = 5;
    int phaseIndex = 0;
    Phase phase = Phase::Preflop;
    bool smallBlindPaid = false;
    bool largeBlindPaid = false;
    int numPlayersChecked = 0;
    int numPlayersCalled = 0;

    QMap<int, Phase> phaseIndices{
        {0, Phase::Preflop},
        {1, Phase::Flop},
        {2, Phase::Turn},
        {3, Phase::River},
        {4, Phase::Showdown}};

    void newGame();
    void newHand();
    void startRound();
    void continueRound(int playerIndex);
    void nextPhase();
    void delayedContinue(int nextPlayer);

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
    /// @brief Helper method to get player number for printing debug messages.
    int playerIndex(vector<shared_ptr<Card>>& target);
    /// @brief Helper method to check if a player index is valid.
    bool validPlayer(int playerIndex);
    /// @brief Helper method that quickly checks if anyone has bid yet this hand.
    bool noBetsYet() { return noBetsYetThisPhase; }

};

#endif // GAME_H
