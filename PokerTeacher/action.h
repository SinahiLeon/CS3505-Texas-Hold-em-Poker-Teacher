#ifndef ACTION_H
#define ACTION_H

#include <variant>
using std::variant;

struct None {};
struct Check {};
struct Call {};
struct Raise {
    int raiseAmount;
};
struct Fold {};

// monostate is null for variants.s
using Action = variant<None, Check, Call, Raise, Fold>;

#endif // ACTION_H
