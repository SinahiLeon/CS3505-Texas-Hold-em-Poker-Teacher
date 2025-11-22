#ifndef HANDTYPE_H
#define HANDTYPE_H

#include <compare>
using std::strong_ordering;

enum class HandType {
    HighCard,
    OnePair,
    TwoPair,
    ThreeKind,
    Straight,
    Flush,
    FullHouse,
    FourKind,
    StraightFlush,
    RoyalFlush
};

#endif // HANDTYPE_H
