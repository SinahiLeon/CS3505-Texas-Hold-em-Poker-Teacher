#include "game.h"

Game::Game(QObject *parent)
    : QObject(parent),
    pot(0),
    currentBet(0),
    smallBlind(5),
    bigBlind(10),
    phase(Phases::Preflop)
{
    players.resize(3);
    players[0].chips = 50;  // You
    players[1].chips = 50;  // Opponent 1
    players[2].chips = 50;  // Opponent 2

    // Start UI with accurate values
    for (int i = 0; i < players.size(); i++) {
        emit chipsUpdated(i, players[i].chips);
    }
    emit potUpdated(pot);
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
