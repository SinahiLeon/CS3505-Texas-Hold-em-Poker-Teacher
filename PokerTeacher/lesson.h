#ifndef LESSON_H
#define LESSON_H

#include <QObject>

/// @brief The model for the infobox and an encapsulation of a game lesson.
class Lesson : public QObject
{
    Q_OBJECT

public:
    Lesson();
};

#endif // LESSON_H
