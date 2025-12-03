#ifndef LESSON_H
#define LESSON_H

#include <QObject>
#include <QDir>
#include <QRegularExpression>
#include <QUrl>
#include "computerplayer.h"
using std::vector;
using std::optional;

/// @brief The model for the infobox and an encapsulation of a game lesson.
class Lesson : public QObject
{
    Q_OBJECT

public:
    explicit Lesson(QString folderPath, QObject *parent = nullptr);
    explicit Lesson(const Lesson& other);
    QString getName() const { return name; };
    QString getCurrentPage() const { return lessonPages[pageIndex]; }
    QUrl getCurrentUrl() const { return QUrl("qrc" + lessonPages[pageIndex]); };
    vector<QString>& getLessonPages() { return lessonPages; };
    vector<ComputerPlayer>& getComputerPlayers() { return cpus; };
    int getLessonNum() const { return lessonNum; };
    bool atStart() const { return pageIndex == 0; }
    bool atEnd() const { return pageIndex == lessonPages.size() - 1; };
    bool hasComupterPlayers() const { return !cpus.empty(); };
    bool back();
    bool nextPage();
    optional<Lesson> getNextLesson();

    bool hasDecision() const { return currentDecision.has_value(); }
    QString getDecisionPrompt() const;
    QStringList getDecisionChoices() const;

    Q_INVOKABLE void makeChoice(int choiceIndex);

signals:
    void pageChanged();
    void choiceResult(bool correct, QString feedback);

private:
    const QDir folder;
    QString name;
    vector<QString> lessonPages;
    vector<ComputerPlayer> cpus;
    int lessonNum;
    int pageIndex;
    const QRegularExpression underscores = QRegularExpression("_");
    const QRegularExpression precedingPath = QRegularExpression("^.*[/]");

    struct Decision {
        QString prompt;
        QStringList choices;
        int correctChoice; // Index of correct answer
        QString correctFeedback;
        QString incorrectFeedback;
    };

    std::optional<Decision> currentDecision;
    void loadDecisionForPage(int pageIndex);

    void readFolderName();
    void findLessonFiles();
    static bool isHtmlFile(QString filename);
    static optional<QString> findNextLesson(QDir parentDir, int index);
    void pigeonHoleSort();
};
#endif // LESSON_H
