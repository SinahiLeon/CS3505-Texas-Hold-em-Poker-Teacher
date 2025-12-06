#ifndef LESSON_H
#define LESSON_H

#include "cardhand.h"
#include "card.h"
#include <QObject>
#include <QDir>
#include <QString>
#include <qregularexpression.h>
#include <vector>
#include <optional>
#include <QMap>
#include <QList>
#include <QUrl>
#include <botaction.h>
#include <queue>
#include "decision.h"

using std::queue;
using std::vector;
using std::optional;

class Lesson : public QObject
{
    Q_OBJECT


public:
    Lesson(QString folderPath, QObject *parent = nullptr);
    Lesson(const Lesson& other);
    Lesson operator=(const Lesson& other);

    bool back();
    bool nextPage();
    optional<Lesson> getNextLesson();

    // Decision handling
    bool hasDecision() const { return currentDecision.has_value(); }
    QString getDecisionPrompt() const;
    QStringList getDecisionChoices() const;

    // Bot actions handling
    bool hasBotActions() const { return !currentBotActions.empty(); }
    vector<BotAction> getCurrentBotActions() const { return currentBotActions; }

    // Call this when user makes a choice
    void makeChoice(int choiceIndex);

    // Getters
    int getCurrentPageIndex() const { return pageIndex + 1; }
    QString getCurrentPage() const;
    QString getName() const { return name; }
    int getLessonNum() const { return lessonNum; }
    vector<QString> getLessonPages() const { return lessonPages; }

    bool atStart() const { return pageIndex == 0; }
    bool atEnd() const { return pageIndex >= lessonPages.size() - 1; }
    QUrl getCurrentUrl() const;
    queue<Action> getPlayerBotActions(int playerIndex) const;

    vector<shared_ptr<Card>> getDeck(int deckNum = 1);

public slots:
    void allowNext(bool allowed);
    void showFeedback(QString feedback);
    void sendDecision();


signals:
    void pageChanged();
    void choiceResult(bool correct, QString feedback, Action action, int amount);
    void newActions();
    void resetGame(int dealerIndex = 0, int deck = -1);
    void updateNext(bool allowed);
    void updateDecision(Decision decision);
    void displayDecision(Decision& decision);
    void displayFeedback(QString feedback);
    void botActionsReady();

private:
    QDir folder;
    QString name;
    std::optional<Decision> currentDecision;
    vector<BotAction> currentBotActions;
    QMap<int, vector<BotAction>> allBotActions;
    vector<QString> lessonPages;
    int lessonNum;
    int pageIndex;
    int greatestIndex = 0;

    // Constants
    const QRegularExpression precedingPath = QRegularExpression("^.*[/]");
    const QChar underscores = '_';

    void readFolderName();
    void findLessonFiles();
    void loadDecisionForPage(int pageIndex);
    void loadBotActionsFromCSV();
    void updateCurrentBotActions();
    bool isHtmlFile(QString filename);
    void pigeonHoleSort();
    optional<QString> findNextLesson(QDir parentDir, int index);
    Action stringToAction(QString s);
};
#endif // LESSON_H
