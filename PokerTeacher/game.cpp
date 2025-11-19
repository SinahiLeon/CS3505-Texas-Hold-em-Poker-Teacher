#include "game.h"

Game::Game(QObject *parent)
    : QObject(parent),
    playerChips(1000),
    computerChips(1000),
    pot(0),
    currentBet(0),
    smallBlind(10),
    bigBlind(20),
    phase(Phases::Preflop)
{
    // TODO: initialize deck, shuffle, etc.
}

// ---------------------------
// TODO: Add game logic methods here
// (deal cards, post blinds, betting, transitions, etc.)
// ---------------------------

