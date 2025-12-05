#include "game.h"
#include "cardlibrary.h"
#include <qdatetime.h>
#include <qdebug.h>
#include <QString>
#include <qthread.h>
#include <random>
#include <algorithm>
#include <QRandomGenerator>
#include <QTimer>
using std::make_shared;
using std::random_device;
using std::default_random_engine;
using std::variant;
using std::get;

Game::Game(QObject *parent) : QObject(parent) {}

void Game::newGame() {
    // Set up start of game
    qDebug() << "Setting up new game...";
    pot = 0;
    players.clear();

    for (int i = 0; i < 3; i++) {
        Player player = Player(i == 0);
        players.push_back(player);
    }

    // Set up start of first hand
    newHand();
    // Start the game loop
    dealerIndex = 2;
    startRound();
}

void Game::startRound() {
    qDebug() << "Starting round with player" << getDealerIndex() << "as dealer.";
    emit dealerUpdate(getDealerIndex());

    // Force small and large blinds if they haven't been paid yet
    int playerIndex = getDealerIndex();
    playerIndex++;

    if (playerIndex >= players.size()) {
        playerIndex = 0;
    }

    makeBet(playerIndex, getSmallBlind(), false);
    smallBlindPaid = true;
    emit updateLastAction(playerIndex, QString("paid the small blind of $%1.").arg(getSmallBlind()));
    playerIndex++;
    if (playerIndex >= players.size()) {
        playerIndex = 0;
    }

    makeBet(playerIndex, getLargeBlind(), false);
    largeBlindPaid = true;
    qDebug() << "Player" << playerIndex << "paid the large blind of" << getLargeBlind();
    emit updateLastAction(playerIndex, QString("paid the large blind: $%1.").arg(getLargeBlind()));

    noBetsYetThisPhase = true;
    continueRound(playerIndex);
}

void Game::newHand() {
    qDebug() << "Setting up new hand...";
    currentBet = 0;
    phaseIndex = 0;

    for (int i = 0; i < players.size(); i++) {
        players[i].resetPlayer();
    }

    currentBet = bigBlind;
    emit currentBetUpdated(currentBet);

    phase = Phase::Preflop;
    emit phaseUpdated(phase);

    communityCards.clear();
    emit communityCardsUpdated();

    shuffleDeck();
    dealHoleCards();
    emit handCardsUpdated();

    emit updateLastAction(1, QString("is waiting."));
    emit updateLastAction(2, QString("is waiting."));
}

void Game::continueRound(int playerIndex) {
    qDebug() << "Continuing round after player" << playerIndex;

    while (true) { // Loop until it's the player's turn
        // Check if game should end
        int activePlayers = 0;
        int lastPlayer = -1;

        for (int i = 0; i < players.size(); i++) {
            if (players[i].canWinPot()) {
                activePlayers++;
                lastPlayer = i;
            }
        }

        if (activePlayers <= 1) {
            emit handEnded(lastPlayer);
            return;
        }

        playerIndex++;

        if (playerIndex >= players.size()) {
            playerIndex = 0;
        }

        if (playerIndex == 0 && !players[playerIndex].getFolded()) {
            break;
        }

        qDebug() << "Player" << playerIndex <<
            "is deciding what to do... noBetsYet =" << noBetsYet() <<
            "and currentBet =" << getBetAmount();

        Action decision = players[playerIndex].makeDecision(currentBet);

        switch (decision) {
            case (Action::None) : { // None
                qDebug() << "Player" << playerIndex << "is having their turn skipped.";
                break;
            }

            case (Action::Check) : { // Check
                thinkingDelay(playerIndex);
                check(playerIndex);
                break;
            }

            case (Action::Call) : { // Call
                thinkingDelay(playerIndex);
                call(playerIndex);
                break;
            }

            case (Action::Raise): { // Raise
                thinkingDelay(playerIndex);
                raise(playerIndex, 10);
                break;
            }

            case (Action::Fold): { // Fold
                thinkingDelay(playerIndex);
                fold(playerIndex);
                break;
            }
        }
    }

    qDebug() << "It's the player's turn...";

    // Update UI
    for (int i = 0; i < players.size(); i++) {
        emit chipsUpdated(i, players[i].getChips(), players[i].getBet());
    }
    emit potUpdated(pot);
    emit currentBetUpdated(currentBet);
    emit updateAvailableActions();
}

void Game::shuffleDeck() {
    qDebug() << "Shuffling deck...";

    // Create a vector of all cards as shared_ptrs
    vector<shared_ptr<Card>> shufflingCards;
    for (const auto& card : CardLibrary::cards) {
        // Create shared_ptr for each card
        shufflingCards.push_back(make_shared<Card>(card.second));
    }

    // Shuffle
    random_device random;
    default_random_engine rng(random());
    shuffle(shufflingCards.begin(), shufflingCards.end(), rng);

    // Add to deck
    clearDeck();
    for (auto& card : shufflingCards) {
        deck.push(std::move(card));
    }
}

void Game::thinkingDelay(int playerIndex) {
    emit updateLastAction(playerIndex, QString("is thinking..."));
    qDebug() << "Starting sleep delay for player" << playerIndex << "thinking...";
    QTime delayTime = QTime::currentTime().addMSecs(1000);
    while (QTime::currentTime() < delayTime) {
        QThread::msleep(50);
    }
}

void Game::rigDeck(vector<shared_ptr<Card>> cardOrder){
    clearDeck();
    for (auto& card : cardOrder) {
        deck.push(std::move(card));
    }
}

void Game::clearDeck() {
    while (!deck.empty()) {
        deck.pop();
    }
}

void Game::dealCards(int cardAmount, vector<shared_ptr<Card>>& target) {
    for(int i = 0; i < cardAmount; i++) {
        if (!deck.empty()) {
            target.push_back(deck.top()); // This copies shared_ptr
            deck.pop();
        }
    }
}

bool Game::validPlayer(int playerIndex) {
    return (playerIndex >= 0 && playerIndex < players.size());
}

void Game::dealHoleCards() {
    qDebug() << "Dealing hole cards...";
    for (auto& player : players) {
        player.heldCards.clear();
        dealCards(2, player.heldCards);
    }
}

void Game::check(int playerIndex) {
    if (!validPlayer(playerIndex))  {
        return;
    }

    qDebug() << "Player" << playerIndex << "checked.";

    emit updateLastAction(playerIndex, QString("checked."));

    numPlayersChecked++;
    if (numPlayersChecked == players.size()) {
        qDebug() << "All players have checked back-to-back. Advancing phase...";
        nextPhase();
    }
}

void Game::makeBet(int playerIndex, int chipAmount, bool isCall) {
    if (!validPlayer(playerIndex)) {
        return;
    }

    if (players[playerIndex].getFolded()) {
        return;
    }

    qDebug() << "Player" << playerIndex << "made a bet of" << chipAmount;

    Player& player = players[playerIndex];

    int playerBet = player.makeBet(chipAmount);
    pot += playerBet;

    numPlayersChecked = 0;
    if (!isCall) numPlayersCalled = 0;
    noBetsYetThisPhase = false;
    emit chipsUpdated(playerIndex, player.getChips(), player.getBet());
    emit potUpdated(pot);
    emit updateLastAction(playerIndex, QString("bet %1.").arg(playerBet));
}

void Game::raise(int playerIndex, int chipAmount) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "raised" << chipAmount;

    Player& player = players[playerIndex];
    int totalBet = currentBet + chipAmount;
    int raiseAmount = totalBet - player.getBet();
    makeBet(playerIndex, raiseAmount, false);

    currentBet = totalBet;
    emit currentBetUpdated(currentBet);
    emit updateLastAction(playerIndex, QString("raised %1. Total bet: %2").arg(chipAmount).arg(totalBet));
}

void Game::call(int playerIndex) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "called at" << currentBet;

    Player& player = players[playerIndex];
    int callAmount = currentBet - player.getBet();

    if (callAmount > 0) {
        makeBet(playerIndex, callAmount, true);
    }

    if (callAmount < 0) {
        makeBet(playerIndex, player.getChips(), true);
    }

    emit updateLastAction(playerIndex, QString("called. Total bet: %1.").arg(player.getBet()));
    numPlayersCalled++;
    int activePlayers = 0;
    for (int i = 0; i < players.size(); i++) {
        if (players[i].canWinPot()) {
            activePlayers++;
        }
    }
    if (numPlayersCalled == activePlayers) {
        qDebug() << "All active players have called back-to-back. Advancing phase...";
        nextPhase();
    }
}

void Game::fold(int playerIndex) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "folded.";

    players[playerIndex].fold();
    emit playerFolded(playerIndex);

    // Check if game should end
    int activePlayers = 0;
    int lastActivePlayer = -1;
    for (int i = 0; i < players.size(); i++) {
        if (players[i].canWinPot()) {
            activePlayers++;
            lastActivePlayer = i;
        }
    }

    if (activePlayers == 1) {
        awardPotToPlayer(lastActivePlayer);
    }
    emit updateLastAction(playerIndex, QString("folded."));
}

int Game::determineIndexOfWinner(){
    int indexOfWinner = -1;
    for (int i = 0; i < players.size(); i++){
        if (players[i].canWinPot()) {
            CardHand currentHand(players[i].heldCards);
            currentHand.calculateBestHand(communityCards);

            if (indexOfWinner == -1) {  // first iteration
                indexOfWinner = i;
            } else {
                CardHand bestSoFar(players[indexOfWinner].heldCards);
                bestSoFar.calculateBestHand(communityCards);

                if (currentHand > bestSoFar) {
                    indexOfWinner = i;
                }
            }
        }
    }
    return indexOfWinner;
}

void Game::awardPotToPlayer(int playerIndex) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Awarding pot of" << pot << "to Player" << playerIndex;
    Player& player = players[playerIndex];
    player.addChips(pot);
    emit chipsUpdated(playerIndex, player.getChips(), player.getBet());

    pot = 0;
    emit potUpdated(pot);
    emit handEnded(playerIndex);
}

void Game::nextPhase() {
    qDebug() << "Moving from" << phaseIndex << "to phase" << phaseIndex+1;
    numPlayersChecked = 0;
    numPlayersCalled = 0;
    noBetsYetThisPhase = false;
    if (phaseIndex < 4) {
        phaseIndex++;
        phase = static_cast<Phase>(phaseIndex);

        // Deal community cards based on phase
        switch (phase) {
        case Phase::Preflop:
            break;
        case Phase::Flop:
            dealCards(3, communityCards);
            break;
        case Phase::Turn:
            dealCards(1, communityCards);
            break;
        case Phase::River:
            dealCards(1, communityCards);
            break;
        case Phase::Showdown:
            if (pot > 0) {
                int winner = determineIndexOfWinner();
                if (winner != -1) {
                    awardPotToPlayer(winner);
                    emit updateLastAction(winner, QString("was awarded the pot."));
                    emit handEnded(winner);
                }
            }
            break;
        }
        emit communityCardsUpdated();
        emit phaseUpdated(phase);
        emit currentBetUpdated(currentBet);
    }
}

void Game::playerMakesBet(int amount) {
    (noBetsYet()) ? makeBet(0, amount, false)
                  : raise(0, amount);
    emit playersTurnEnded();
    continueRound(0);
}

void Game::playerCalls() {
    call(0);
    emit playersTurnEnded();
    continueRound(0);
}

void Game::playerChecks() {
    check(0);
    emit playersTurnEnded();
    continueRound(0);
}

void Game::playerFolds() {
    fold(0);
    emit playersTurnEnded();
    continueRound(0);
}

void Game::onViewInitialized() {
    newGame();
}

void Game::startNextHand() {
    // Set up start of next hand
    newHand();
    // Start the game loop with next dealer
    dealerIndex++;
    if (dealerIndex >= players.size()) {
        dealerIndex = 0;
    }
    startRound();
}
