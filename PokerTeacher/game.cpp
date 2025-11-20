#include "game.h"
#include "cardlibrary.h"
#include <qdebug.h>
#include <QString>
#include <random>

Game::Game(QObject *parent)
    : QObject(parent),
    pot(0),
    currentBet(0),
    smallBlind(5),
    bigBlind(10),
    phase(Phases::Preflop)
{
    shuffleDeck();
}

void Game::shuffleDeck() {
    qDebug() << "Shuffling deck...";
    // Get pointers to all cards in library
    std::vector<const std::pair<const QString, Card>*> shufflingCards;  // vector< pair<qstring, card>* >
    for (const auto& pair : CardLibrary::cards) {
        shufflingCards.push_back(&pair);
    }
    // Shuffle using random_device
    std::random_device random;
    std::default_random_engine rng(random());
    std::shuffle(shufflingCards.begin(), shufflingCards.end(), rng);
    // Add all shuffled cards to the deck
    clearDeck();
    for (const auto& card : shufflingCards) {
        deck.push(card);
    }
    qDebug() << "Top of deck:" << deck.top()->first;
    // Card order for debugging
    std::vector<QString> cardOrder;
    for (auto& card : shufflingCards) {
        cardOrder.push_back(card->first);
    }
    std::reverse(cardOrder.begin(), cardOrder.end());
    qDebug() << "New card order:" << cardOrder;
}

void Game::clearDeck() {
    while (!deck.empty()) {
        deck.pop();
    }
}

void Game::newGame() {
    players.resize(3);
    players[0].chips = 50;  // You
    players[1].chips = 50;  // Opponent 1
    players[2].chips = 50;  // Opponent 2

    phase = Phases::Preflop;

    // Start UI with accurate values
    for (int i = 0; i < players.size(); i++) {
        emit chipsUpdated(i, players[i].chips);
    }
    emit potUpdated(pot);
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

void Game::nextPhase(){
    if(phaseIndex == 4){
        phaseIndex = 0;
        phase = phaseIndices[phaseIndex];
    }
    else{
        phaseIndex++;
        phase = phaseIndices[phaseIndex];
    }
}

