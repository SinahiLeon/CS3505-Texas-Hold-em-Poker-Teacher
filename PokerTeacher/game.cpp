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
    shuffleDeck();
}

void Game::shuffleDeck() {
    qDebug() << "Shuffling deck...";

    // Create a vector of all cards as shared_ptrs
    vector<shared_ptr<Card>> shufflingCards;
    for (const auto& pair : CardLibrary::cards) {
        // Create shared_ptr for each card
        shufflingCards.push_back(make_shared<Card>(pair.second));
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
    players.clear();
    players[0].chips = 50;
    players[1].chips = 50;
    players[2].chips = 50;

    communityCards.clear();
    pot = 0;
    currentBet = 0;
    phaseIndex = 0;
    phase = Phases::Preflop;

    shuffleDeck();
    dealHoleCards();

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
}

void Game::dealCards(int cardAmount, vector<shared_ptr<Card>>& target) {
    for(int i = 0; i < cardAmount; i++) {
        if (!deck.empty()) {
            target.push_back(deck.top()); // This copies shared_ptr
            deck.pop();
        }
    }
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

    getPlayer(playerIndex).chips -= chipAmount;
    pot += chipAmount;
    emit chipsUpdated(playerIndex, players[playerIndex].chips);
    emit potUpdated(pot);
}

//call()
//raise()



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
        case Phases::Flop:
            dealCards(3, communityCards);
            break;
        case Phases::Turn:
        case Phases::River:
            dealCards(1, communityCards);
            break;
        case Phases::Showdown:
            break;
        }

        if (phase != Phases::Showdown) {

        }

        emit communityCardsUpdated();
        emit phaseUpdated(phase);
        emit currentBetUpdated(currentBet);
    }
}

