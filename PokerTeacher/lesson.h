#ifndef LESSON_H
#define LESSON_H

#include <QObject>
#include "computerplayer.h"
using std::vector;

/// @brief The model for the infobox and an encapsulation of a game lesson.
class Lesson : public QObject
{
    Q_OBJECT

public:
    explicit Lesson(QString folderPath, QObject *parent = nullptr);
    string getName() { return name; };
    vector<QString>& getLessonFiles() { return lessonFiles; };
    vector<ComputerPlayer> getComputerPlayers() { return cpus; };
private:
    string name;
    vector<QString> lessonFiles;
    vector<ComputerPlayer> cpus;
};

#endif // LESSON_H
