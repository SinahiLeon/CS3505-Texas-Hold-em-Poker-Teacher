#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "deck.h"
#include <QObject>
#include <vector>


class Game : public QObject
{
    Q_OBJECT
public:
    std::vector<Card> river;
    explicit Game(QObject *parent = nullptr);

private:
    Deck deck;

signals:
};

#endif // GAME_H
