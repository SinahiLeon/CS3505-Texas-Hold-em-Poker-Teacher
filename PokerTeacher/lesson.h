#ifndef LESSON_H
#define LESSON_H

#include <QObject>
#include <QDir>
#include <QString>
#include <vector>
#include <optional>
#include <QMap>
#include <QList>
#include <QUrl>

class Game;

struct BotAction {
    int page;
    int player;
    QString action;
    int amount;
};

class Lesson : public QObject
{
    Q_OBJECT


public:
    Lesson(QString folderPath, QObject *parent = nullptr);
    Lesson(const Lesson& other);

    bool back();
    bool nextPage();
    std::optional<Lesson> getNextLesson();

    // Decision handling
    bool hasDecision() const { return currentDecision.has_value(); }
    QString getDecisionPrompt() const;
    QStringList getDecisionChoices() const;

    // Bot actions handling
    bool hasBotActions() const { return !currentBotActions.isEmpty(); }
    QList<BotAction> getCurrentBotActions() const { return currentBotActions; }

    // Call this when user makes a choice
    Q_INVOKABLE void makeChoice(int choiceIndex);

    // Apply bot actions to game
    void applyBotActionsToGame(Game* game);

    // Getters
    int getCurrentPageIndex() const { return pageIndex + 1; }
    QString getCurrentPage() const;
    QString getName() const { return name; }
    int getLessonNum() const { return lessonNum; }
    std::vector<QString> lessonPages;

    bool atStart() const { return pageIndex == 0; }
    bool atEnd() const { return pageIndex >= lessonPages.size() - 1; }
    QUrl getCurrentUrl() const {
        qDebug() << QUrl("qrc" + lessonPages[pageIndex]);
        return QUrl("qrc" + lessonPages[pageIndex]);

    };


signals:
    void pageChanged();
    void choiceResult(bool correct, QString feedback);
    void botActionsReady();

private:
    struct Decision {
        QString prompt;
        QStringList choices;
        int correctChoice;
        QString correctFeedback;
        QString incorrectFeedback;
    };

    QDir folder;
    QString name;
    std::optional<Decision> currentDecision;
    QList<BotAction> currentBotActions;
    QMap<int, QList<BotAction>> allBotActions;
    int lessonNum;
    int pageIndex;

    // Constants
    static const QString precedingPath;
    static const QChar underscores;

    void readFolderName();
    void findLessonFiles();
    void loadDecisionForPage(int pageIndex);
    void loadBotActionsFromCSV();
    void updateCurrentBotActions();
    bool isHtmlFile(QString filename);
    void pigeonHoleSort();
    std::optional<QString> findNextLesson(QDir parentDir, int index);
};
#endif // LESSON_H
