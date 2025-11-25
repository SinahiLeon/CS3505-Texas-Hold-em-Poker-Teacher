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
    dealerIndex = 0;
    newHand();
    // Start the game loop
    continueRound();
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

void Game::continueRound() {
    qDebug() << "Playing round with player" << getDealerIndex() << "as dealer.";
    int player = getDealerIndex() + 1;
    if (player >= players.size()) player = 0; // Loop back to player
    while (player != 0) { // While it's a bot's turn
        // Bot chooses action based on its held cards:
        /* TEMPORARY RANDOMIZER */
        /* REPLACE THIS WITH DECISION CODE */
        int choice = QRandomGenerator::global()->bounded(0, 2);
        if (choice == 0) {
            if (noBetsYet()) check(player);
            else call(player); }
        if (choice == 1) {
            if (noBetsYet()) makeBet(player, getCurrentBet());
            else raise(player, getCurrentBet()); }
        if (choice == 2) fold(player);
        /* END OF TEMPORARY RANDOMIZER */

        // Go to next bot's turn
        player++;
        if (player >= players.size()) player = 0; // Loop back to player
    }

    // Update UI
    for (int i = 0; i < players.size(); i++) {
        emit chipsUpdated(i, players[i].chips);
    }
    emit potUpdated(pot);
    emit currentBetUpdated(currentBet);
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
}

void Game::makeBet(int playerIndex, int chipAmount) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "made a bet of" << chipAmount;

    Player& player = players[playerIndex];
    int actualBet = std::min(chipAmount, player.chips);

    player.chips -= actualBet;
    player.currentBet += actualBet;
    pot += actualBet;

    emit chipsUpdated(playerIndex, players[playerIndex].chips);
    emit potUpdated(pot);
    emit updateLastAction(playerIndex, QString("bet %1.").arg(actualBet));
}

void Game::call(int playerIndex) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "called at" << currentBet;

    Player& player = players[playerIndex];
    int callAmount = currentBet - player.currentBet;
    if (callAmount > 0) {
        makeBet(playerIndex, callAmount);
    }
    emit updateLastAction(playerIndex, QString("called."));
}

void Game::raise(int playerIndex, int chipAmount) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Player" << playerIndex << "raised" << chipAmount;

    int totalBet = players[playerIndex].currentBet + chipAmount;
    if (totalBet > currentBet) {
        currentBet = totalBet;
        makeBet(playerIndex, chipAmount);
        emit currentBetUpdated(currentBet);
    }
    emit updateLastAction(playerIndex, QString("raised %1.").arg(chipAmount));
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
        nextPhase(); // Move to showdown
    }
    emit updateLastAction(playerIndex, QString("folded."));
}

void Game::awardPotToPlayer(int playerIndex) {
    if (!validPlayer(playerIndex)) return;
    qDebug() << "Awarding pot of" << pot << "to Player" << playerIndex;

    players[playerIndex].chips += pot;
    emit chipsUpdated(playerIndex, players[playerIndex].chips);

    pot = 0;
    emit potUpdated(pot);
}

void Game::nextPhase() {
    qDebug() << "Moving from" << phaseIndex << "to phase" << phaseIndex+1;
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
            break;
        }

        if (phase != Phase::Showdown) {

        }

        emit communityCardsUpdated();
        emit phaseUpdated(phase);
        emit currentBetUpdated(currentBet);
    }
}

bool Game::noBetsYet() {
    return getCurrentBet() == bigBlind;
}

void Game::onViewInitialized() {
    newGame();
}
