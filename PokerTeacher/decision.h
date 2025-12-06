#ifndef DECISION_H
#define DECISION_H

#include <QString>
#include <QStringList>
#include "action.h"

struct Decision {
    QString prompt;
    QStringList choices;
    int correctChoice;
    QString correctFeedback;
    QString incorrectFeedback;
    Action correctAction;
};

#endif // DECISION_H
