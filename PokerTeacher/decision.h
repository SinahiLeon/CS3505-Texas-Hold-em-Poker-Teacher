#ifndef DECISION_H
#define DECISION_H

#include <QString>
#include <QStringList>
#include "action.h"

struct Decision {
    QString prompt;
    QStringList choices;
    int correctChoice;
    QString incorrectFeedback;
    Action correctAction;
    int amount;
};

#endif // DECISION_H
