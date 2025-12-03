#include "game.h"
#include "cardlibrary.h"
#include <qdebug.h>
#include <QString>
#include <random>
#include <algorithm>
#include <QRandomGenerator>
using std::make_shared;
using std::random_device;
using std::default_random_engine;

Game::Game(QObject *parent) : QObject(parent) {}

void Game::newGame() {
    // Set up start of game
    qDebug() << "Setting up new game...";
    pot = 0;
    players.clear();
    for (int i = 0; i < 3; i++) {
        Player player;
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
    int player = getDealerIndex();
    player++;
    if (player >= players.size()) player = 0; // Loop back to user
    makeBet(player, getSmallBlind());
    smallBlindPaid = true;
    qDebug() << "Player" << player << "paid the small blind of" << getSmallBlind();
    emit updateLastAction(player, QString("paid the small blind: $%1.").arg(getSmallBlind()));
    player++;
    makeBet(player, getLargeBlind());
    largeBlindPaid = true;
    qDebug() << "Player" << player << "paid the large blind of" << getLargeBlind();
    emit updateLastAction(player, QString("paid the large blind: $%1.").arg(getLargeBlind()));
    noBetsYetThisPhase = true;
    continueRound(player);
}

void Game::newHand() {
    qDebug() << "Setting up new hand...";
    currentBet = 0;
    phaseIndex = 0;

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
    int player = playerIndex;
    while (true) { // Loop until it's the player's turn
        player++;
        if (player >= players.size()) player = 0; // Loop back to user
        if (player != 0) {
            if (players[player].folded) {
                qDebug() << "Player" << player << "is folded. Skipping...";
                continue;
            }
            qDebug() << "Opponent" << player <<
                "is deciding what to do... noBetsYet =" << noBetsYet() <<
                "and currentBet =" << getBetAmount();
            // Bot chooses action based on its held cards:

            players[player].makeDecision();
            /* TEMPORARY RANDOMIZER */
            /* REPLACE QRandomGenerator WITH DECISION CODE */
            if (noBetsYet()) {
                if (players[player].canBet(getBetAmount())) {
                    switch (int choice = QRandomGenerator::global()->bounded(0, 2)) {
                        case (0) : { check(player); continue; }
                        case (1) : { makeBet(player, getBetAmount()); continue; }
                        case (2) : { fold(player); continue; }
                    }
                }
                else { // No bets yet but player can't bet
                    switch (int choice = QRandomGenerator::global()->bounded(0, 1)) {
                        case (0) : { check(player); continue; }
                        case (1) : { fold(player); continue; }
                    }
                }
            }
            else { // There has been a bet
                if (players[player].canBet(getRaiseAmount())) { // Can raise
                    switch (int choice = QRandomGenerator::global()->bounded(0, 2)) {
                        case (0) : { call(player); continue; }
                        case (1) : { raise(player, getLargeBlind()); continue; }
                        case (2) : { fold(player); continue; }
                    }
                }
                if (players[player].canBet(getBetAmount())) { // Can call
                    switch (int choice = QRandomGenerator::global()->bounded(0, 1)) {
                    case (0) : { call(player); continue; }
                    case (1) : { fold(player); continue; }
                    }
                }
                else { // Can't check, call, or raise
                    fold(player);
                    continue;
                }
            }
            /* END OF TEMPORARY RANDOMIZER */
        }
        else break;
    }
    qDebug() << "It's the player's turn...";

    // Update UI
    for (int i = 0; i < players.size(); i++) {
        emit chipsUpdated(i, players[i].chips, players[i].currentBet);
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

void Game::clearDeck() {
    while (!deck.empty()) {
        deck.pop();
    }
}

void Game::dealCards(int cardAmount, vector<shared_ptr<Card>>& target) {
    for(int i = 0; i < cardAmount; i++) {
        if (!deck.empty()) {
            target.push_back(deck.top()); // This copies shared_ptr
            qDebug() << "Dealt" << deck.top()->name << "to player" << playerIndex(target) << "with" << deck.top()->image;
            deck.pop();
        }
    }
}

int Game::playerIndex(vector<shared_ptr<Card>>& target) {
    for (int i = 0; i < players.size(); i++)
        if (&target == &players.at(i).heldCards)
            return i;
    return -1;
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
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "checked.";
    emit updateLastAction(playerIndex, QString("checked."));
    numPlayersChecked++;
    if (numPlayersChecked == players.size()) {
        qDebug() << "All players have checked back-to-back. Advancing phase...";
        nextPhase();
    }
}

void Game::makeBet(int playerIndex, int chipAmount) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "made a bet of" << chipAmount;

    Player& player = players[playerIndex];
    int actualBet = std::min(chipAmount, player.chips);

    player.chips -= actualBet;
    player.currentBet += actualBet;
    pot += actualBet;

    numPlayersChecked = 0;
    numPlayersCalled = 0;
    noBetsYetThisPhase = false;
    emit chipsUpdated(playerIndex, player.chips, player.currentBet);
    emit potUpdated(pot);
    emit updateLastAction(playerIndex, QString("bet %1.").arg(actualBet));
}

void Game::raise(int playerIndex, int chipAmount) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "raised" << chipAmount;

    Player& player = players[playerIndex];
    int totalBet = currentBet + chipAmount;
    int raiseAmount = totalBet - player.currentBet;
    if (totalBet > currentBet) {
        makeBet(playerIndex, raiseAmount);
        currentBet = totalBet;
        emit currentBetUpdated(currentBet);
    }
    emit updateLastAction(playerIndex, QString("raised %1. Total bet: %2").arg(chipAmount).arg(totalBet));
}

void Game::call(int playerIndex) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "called at" << currentBet;

    Player& player = players[playerIndex];
    int callAmount = currentBet - player.currentBet;
    if (callAmount > 0) {
        makeBet(playerIndex, callAmount);
    }
    emit updateLastAction(playerIndex, QString("called. Total bet: %1.").arg(player.currentBet));
    numPlayersCalled++;
    if (numPlayersCalled == players.size()) {
        qDebug() << "All players have called back-to-back. Advancing phase...";
        nextPhase();
    }
}

void Game::fold(int playerIndex) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "folded.";

    players[playerIndex].folded = true;
    emit playerFolded(playerIndex);

    // Check if game should end
    int activePlayers = 0;
    int lastActivePlayer = -1;
    for (int i = 0; i < players.size(); i++) {
        if (!players[i].folded) {
            activePlayers++;
            lastActivePlayer = i;
        }
    }

    if (activePlayers == 1) {
        awardPotToPlayer(lastActivePlayer);
        while (phase != Phase::Showdown)
            nextPhase(); // Move to showdown
    }
    emit updateLastAction(playerIndex, QString("folded."));
}

int Game::determineIndexOfWinner(){
    int indexOfWinner = -1;
    for (int i = 0; i < players.size(); i++){
        if (!players[i].folded) {
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
    player.chips += pot;
    emit chipsUpdated(playerIndex, player.chips, player.currentBet);

    pot = 0;
    emit potUpdated(pot);
}

void Game::nextPhase() {
    qDebug() << "Moving from" << phaseIndex << "to phase" << phaseIndex+1;
    numPlayersChecked = 0;
    numPlayersCalled = 0;
    noBetsYetThisPhase = false;
    if (phaseIndex < 4) {
        phaseIndex++;
        phase = phaseIndices[phaseIndex];

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
                    emit updateLastAction(winner, QString(" was awarded the pot."));
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
    (noBetsYet()) ? makeBet(0, amount)
                  : raise(0, amount);
    continueRound(0);
}

void Game::playerCalls() {
    call(0);
    continueRound(0);
}

void Game::playerChecks() {
    check(0);
    continueRound(0);
}

void Game::playerFolds() {
    fold(0);
    continueRound(0);
}

void Game::onViewInitialized() {
    newGame();
}

void Game::startNewGame() {
    newGame();
}
