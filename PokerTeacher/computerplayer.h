#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "player.h"
#include "action.h"

class ComputerPlayer : public Player
{
public:
    ComputerPlayer(vector<Action> actions);
private:
    vector<Action> actions;
};

#endif // COMPUTERPLAYER_H
