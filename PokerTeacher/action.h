#ifndef ACTION_H
#define ACTION_H

#include <variant>
using std::variant;

struct Check {};
struct Raise {
    int raiseAmount;
};
struct Call {};
struct Fold {};

using Action = variant<Check, Raise, Call, Fold>;

#endif // ACTION_H
