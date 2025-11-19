#ifndef GAME_H
#define GAME_H

#include "card.h"
#include <QObject>
#include <stack>
#include <vector>


class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    std::vector<Card*> river;
    std::vector<Card*> playerHand;
    std::vector<Card*> computerHand;

signals:

private:
    std::stack<Card*> deck;
};

#endif // GAME_H
