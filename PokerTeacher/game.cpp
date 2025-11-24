#include "game.h"
#include "cardlibrary.h"
#include <qdebug.h>
#include <QString>
#include <random>
#include <algorithm>
using std::make_shared;
using std::random_device;
using std::default_random_engine;

Game::Game(QObject *parent)
    : QObject(parent)
{

}

void Game::shuffleDeck() {
    qDebug() << "Shuffling deck...";

    // Create a vector of all cards as shared_ptrs
    vector<shared_ptr<Card>> shufflingCards;
    for (const auto& card : CardLibrary::cards) {
        // Create shared_ptr for each card
        shufflingCards.push_back(make_shared<Card>(card));
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

    qDebug() << "Deck shuffled with" << deck.size() << "cards";
}

void Game::clearDeck() {
    while (!deck.empty()) {
        deck.pop();
    }
}

void Game::newGame() {
    qDebug() << "New game: Creating new players...";
    players.clear();
    for (int i = 0; i < 3; i++) {
        Player player;
        players.push_back(player);
    }

    qDebug() << "New game: Setting up phase...";
    communityCards.clear();
    pot = 0;
    currentBet = 0;
    phaseIndex = 0;
    phase = Phase::Preflop;

    shuffleDeck();
    dealHoleCards();
    emit handCardsUpdated();

    // Apply blinds
    makeBet(0, smallBlind);  // Small blind
    makeBet(1, bigBlind);    // Big blind
    currentBet = bigBlind;

    // Update UI
    for (int i = 0; i < players.size(); i++) {
        emit chipsUpdated(i, players[i].chips);
    }
    emit potUpdated(pot);
    emit currentBetUpdated(currentBet);
    emit phaseUpdated(phase);
    emit communityCardsUpdated();
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

void Game::dealHoleCards() {
    for (auto& player : players) {
        player.heldCards.clear();
        dealCards(2, player.heldCards);
    }
}

void Game::makeBet(int playerIndex, int chipAmount) {
    if (playerIndex < 0 || playerIndex < players.size()) {
        return;
    }

    Player& player = players[playerIndex];
    int actualBet = std::min(chipAmount, player.chips);

    player.chips -= actualBet;
    player.currentBet += actualBet;
    pot += actualBet;

    emit chipsUpdated(playerIndex, players[playerIndex].chips);
    emit potUpdated(pot);
}

void Game::call(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= players.size()) {
        return;
    }

    Player& player = players[playerIndex];
    int callAmount = currentBet - player.currentBet;
    if (callAmount > 0) {
        makeBet(playerIndex, callAmount);
    }
}

void Game::raise(int playerIndex, int chipAmount) {
    if (playerIndex < 0 || playerIndex >= players.size()) {
        return;
    }

    int totalBet = players[playerIndex].currentBet + chipAmount;
    if (totalBet > currentBet) {
        currentBet = totalBet;
        makeBet(playerIndex, chipAmount);
        emit currentBetUpdated(currentBet);
    }
}

void Game::fold(int playerIndex) {
    if (playerIndex < 0 || playerIndex >= players.size()) {
        return;
    }

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
}

// Gives pot to the winner (used inside fold or showdown)
void Game::awardPotToPlayer(int playerIndex) {
    if (playerIndex < 0 || playerIndex < players.size()) {
        return;
    }

    players[playerIndex].chips += pot;
    emit chipsUpdated(playerIndex, players[playerIndex].chips);

    pot = 0;
    emit potUpdated(pot);
}

void Game::nextPhase() {
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

void Game::onViewInitialized(){
    newGame();
}
