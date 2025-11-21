#ifndef CARDVALUE_H
#define CARDVALUE_H

enum class CardValue {
    // Values auto-increment
    Two = 2,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

int operator-(const CardValue& l, const CardValue& r) {
    return static_cast<int>(l) - static_cast<int>(r);
}

int operator-(const CardValue& l, int r) {
    return static_cast<int>(l) - r;
}

#endif // CARDVALUE_H
