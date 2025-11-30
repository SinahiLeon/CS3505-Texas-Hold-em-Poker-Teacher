#ifndef ACTION_H
#define ACTION_H

#include <variant>

struct Check {};
struct Raise {
    int raiseAmount;
};
struct Call {};
struct Fold {};

using Action = std::variant<Check, Raise, Call, Fold>;

#endif // ACTION_H
