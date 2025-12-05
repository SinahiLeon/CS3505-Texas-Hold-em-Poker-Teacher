#ifndef BOTACTION_H
#define BOTACTION_H

#include <QString>
#include "action.h"

struct BotAction {
    int page;
    int player;
    Action action;
    int amount;
};

#endif // BOTACTION_H
