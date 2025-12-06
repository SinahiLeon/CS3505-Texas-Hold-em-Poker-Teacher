#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "cardhand.h"
#include <QObject>
#include <stack>
#include <QMap>
#include "phase.h"
#include "player.h"
#include "lesson.h"

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
    Lesson& getCurrentLesson() { return lesson; }
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
    void chooseLesson(int index);
    /// @brief Compares players best hands to determine winner.
    /// @return int of the player who will be awarded the pot.
    int determineIndexOfWinner();
    Phase getPhase() const { return phase; }
    bool noBetsYetThisPhase;

signals:
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
    void playersTurnEnded();
    void displayFeedback(QString feedback);

public slots:
    void onViewInitialized();
    void playerMakesBet(int amount);
    void playerCalls();
    void playerChecks();
    void playerFolds();
    void startNextHand();
    void startFreeplay();
    void applyLessonBotActions();
    void lessonResetBoard(int dealerIndex = 0, int riggedDeck = -1);

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
    Lesson lesson;

    void newGame();
    void newHand(vector<shared_ptr<Card>> cardOrder = vector<shared_ptr<Card>>());
    void startRound();
    void continueRound(int playerIndex);
    void nextPhase();
    void thinkingDelay(int playerIndex);

    void resetBoard(int dealerIndex, int riggedDeck);

    /// @brief Resets and shuffles the deck with all cards
    void shuffleDeck();
    /// @brief Gets the order of cards for the selected lesson.
    /// @return A vector of cards that is in the order needed for the lesson.
    vector<shared_ptr<Card>> getLessonDeck();
    /// @brief Stacks the deck for the purpose of lessons
    void rigDeck(vector<shared_ptr<Card>> cardOrder);
    /// @brief Helper method to empty the deck stack
    void clearDeck();
    /// @brief Gives pot to the winner (used inside fold or showdown)
    void awardPotToPlayer(int playerIndex); // why are there two of these?
    /// @brief
    void dealCards(int cardAmount, vector<shared_ptr<Card>>& target);
    /// @brief
    void dealHoleCards();
    /// @brief Helper method to check if a player index is valid.
    bool validPlayer(int playerIndex);
    /// @brief Helper method that quickly checks if anyone has bid yet this hand.
    bool noBetsYet() { return noBetsYetThisPhase; }

private slots:
    void getNewActions();
    void recieveDecision(bool correct, QString feedback, Action action, int amount);
};

#endif // GAME_H
