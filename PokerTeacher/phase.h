#ifndef PHASE_H
#define PHASE_H

/// @brief Different phases of Texas Hold'em poker
enum class Phase {
    Preflop,
    Flop,   // Three community cards are dealt in the middle of the table.
    Turn ,   // Fourth card card dealt.
    River,  // The fifth and final community card is dealt.
    Showdown,   // If more than one player remains after the last betting round, players
    // reveal their hands. The player with the best five-card hand wins the pot.
};

#endif // PHASE_H
