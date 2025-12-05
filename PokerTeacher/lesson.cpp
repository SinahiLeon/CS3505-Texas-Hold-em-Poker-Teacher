#include "lesson.h"
#include "cardlibrary.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QFileInfo>
#include <stdexcept>
#include <QDebug>

using std::bad_cast;
using std::out_of_range;
using std::make_shared;
using std::nullopt;

Lesson::Lesson(QString folderPath, QObject *parent): QObject(parent), folder(QDir(folderPath)) {
    readFolderName();
    findLessonFiles();
    pageIndex = 0;
    currentDecision = std::nullopt;
    loadDecisionForPage(pageIndex);
    loadBotActionsFromCSV();
    updateCurrentBotActions();
}

Lesson::Lesson(const Lesson& other)
    : folder(other.folder)
    , name(other.name)
    , lessonPages(other.lessonPages)
    , allBotActions(other.allBotActions)
    , currentBotActions(other.currentBotActions)
    , lessonNum(other.lessonNum)
{
    pageIndex = 0;
    loadDecisionForPage(pageIndex);
    updateCurrentBotActions();
}

Lesson Lesson::operator=(const Lesson& other) {
    if (this == &other) {
        return *this;
    }

    folder = other.folder;
    name = other.name;
    lessonPages = other.lessonPages;
    allBotActions = other.allBotActions;
    currentBotActions = other.currentBotActions;
    lessonNum = other.lessonNum;

    pageIndex = 0;
    loadDecisionForPage(pageIndex);
    updateCurrentBotActions();

    return *this;
}

bool Lesson::back() {
    if (pageIndex <= 0) {
        return false;
    }

    pageIndex--;
    emit pageChanged();
    return true;
}

bool Lesson::nextPage() {
    if (pageIndex >= lessonPages.size() - 1) {
        return false;
    }

    pageIndex++;
    loadDecisionForPage(pageIndex);
    updateCurrentBotActions();
    emit pageChanged();
    emit newActions();
    return true;
}

QUrl Lesson::getCurrentUrl() const {
    qDebug() << QUrl("qrc" + lessonPages[pageIndex]);
    return QUrl("qrc" + lessonPages[pageIndex]);
};

void Lesson::loadDecisionForPage(int pageIndex) {
    currentDecision = std::nullopt;

    // Look for a JSON decision file matching the HTML file
    QString htmlPath = lessonPages[pageIndex];
    QString baseName = QFileInfo(htmlPath).completeBaseName();
    QString jsonPath = folder.absoluteFilePath(baseName + ".json");

    QFile jsonFile(jsonPath);
    if (!jsonFile.exists()) {
        return;
    }

    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open decision file:" << jsonPath;
        return;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON in decision file:" << jsonPath;
        return;
    }

    QJsonObject obj = doc.object();
    Decision decision;

    decision.prompt = obj["prompt"].toString();
    decision.correctChoice = obj["correct"].toInt();
    decision.correctFeedback = obj["correctFeedback"].toString();
    decision.incorrectFeedback = obj["incorrectFeedback"].toString();
    decision.correctAction = stringToAction(obj["correctAction"].toString());

    QJsonArray choicesArray = obj["choices"].toArray();

    for (const QJsonValue& choice : choicesArray) {
        decision.choices.append(choice.toString());
    }

    if (decision.choices.isEmpty() ||
        decision.correctChoice < 0 ||
        decision.correctChoice >= decision.choices.size()) {
        qWarning() << "Invalid decision data in:" << jsonPath;
        return;
    }

    currentDecision = decision;
}

void Lesson::loadBotActionsFromCSV() {
    QString csvPath = folder.absoluteFilePath("actions.csv");
    QFile csvFile(csvPath);

    if (!csvFile.exists()) {
        return;
    }

    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open actions CSV:" << csvPath;
        return;
    }

    QTextStream in(&csvFile);

    if (!in.atEnd()) {
        in.readLine();
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(",");
        if (parts.size() != 4) {
            qWarning() << "Invalid CSV line, expected 4 parts:" << line;
            continue;
        }

        BotAction action;
        bool ok;

        int csvPage = parts[0].toInt(&ok);
        if (!ok) {
            qWarning() << "Invalid page number:" << parts[0];
            continue;
        }
        action.page = csvPage - 1;

        action.player = parts[1].toInt(&ok);
        if (!ok || action.player < 0 || action.player > 2) {
            qWarning() << "Invalid player number:" << parts[1];
            continue;
        }

        action.action = stringToAction(parts[2].trimmed().toLower());
        if (action.action != Action::Fold && action.action != Action::Call &&
            action.action != Action::Raise && action.action != Action::Check) {
            qWarning() << "Invalid action:" << parts[2];
            continue;
        }

        action.amount = parts[3].toInt(&ok);
        if (!ok || action.amount < 0) {
            qWarning() << "Invalid amount:" << parts[3];
            continue;
        }

        allBotActions[action.page].push_back(action);
    }

    csvFile.close();
    qDebug() << "Loaded" << allBotActions.size() << "pages of bot actions";
}

void Lesson::updateCurrentBotActions() {
    currentBotActions.clear();

    if (allBotActions.contains(pageIndex)) {
        currentBotActions = allBotActions[pageIndex];
        qDebug() << "Page" << (pageIndex + 1) << "has" << currentBotActions.size() << "bot actions";
    }
}

}

vector<shared_ptr<Card>> Lesson::getDeck() {
    vector<shared_ptr<Card>> deck;
    QFile inputFile = QFile(folder.filesystemAbsolutePath().append("deck.txt"));
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QString name = Card::toString((line.split(",")[0]).toInt(), line.split(",")[1].toInt());
            deck.push_back(make_shared<Card>(CardLibrary::cards.at(name)));
        }
        inputFile.close();
    }
    return deck;

QString Lesson::getDecisionPrompt() const {
    if (currentDecision.has_value()) {
        return currentDecision->prompt;
    }

    return "Decision could not be found!";
}

QStringList Lesson::getDecisionChoices() const {
    if (currentDecision.has_value()) {
        return currentDecision->choices;
    }

    return QStringList();
}

void Lesson::makeChoice(int choiceIndex) {
    if (!currentDecision.has_value()) {
        return;
    }

    const Decision& decision = currentDecision.value();

    if (choiceIndex < 0 || choiceIndex >= decision.choices.size()) {
        return;
    }

    bool correct = (choiceIndex == decision.correctChoice);
    QString feedback = correct ? decision.correctFeedback : decision.incorrectFeedback;

    emit choiceResult(correct, feedback, decision.correctAction);
}

QString Lesson::getCurrentPage() const {
    if (pageIndex >= 0 && pageIndex < lessonPages.size()) {
        return lessonPages[pageIndex];
    }

    return "";
}

std::optional<Lesson> Lesson::getNextLesson() {
    int nextIndex = lessonNum + 1;
    QDir parent = folder;
    parent.cdUp();

    std::optional<QString> nextPath = findNextLesson(parent, nextIndex);

    if (nextPath.has_value()) {
        return std::optional<Lesson>(Lesson(*nextPath));
    }

    return nullopt;
}

void Lesson::readFolderName() {
    QString folderName = folder.dirName();
    QStringList nameParts = folderName.split("-");

    if (nameParts.size() < 2) {
        throw out_of_range("Lesson folder name " + folderName.toStdString() + " is not valid.");
    }

    bool isNum;
    int num = nameParts[0].toInt(&isNum);

    if (!isNum) {
        throw bad_cast();
    }

    lessonNum = num;
    name = nameParts[1].replace(underscores, " ");
}

void Lesson::findLessonFiles() {
    QStringList allFiles = folder.entryList(QDir::Files);

    for (auto& file : allFiles) {
        if (isHtmlFile(file)) {
            lessonPages.push_back(folder.absoluteFilePath(file));
        }
    }

    pigeonHoleSort();
}

bool Lesson::isHtmlFile(QString filename) {
    QStringList fileParts = filename.split(".");

    if (fileParts.size() > 1 && fileParts[fileParts.size() - 1] == "html") {
        return true;
    }

    return false;
}

std::optional<QString> Lesson::findNextLesson(QDir parentDir, int index) {
    QStringList allFolders = parentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (auto& folderPath : allFolders) {
        QStringList nameParts = folderPath.split("-");

        if (nameParts.size() > 0) {
            bool isNum;
            int num = nameParts[0].toInt(&isNum);

            if (isNum && num == index) {
                return parentDir.absoluteFilePath(folderPath);
            }
        }
    }

    return nullopt;
}

void Lesson::pigeonHoleSort() {
    std::vector<QString> pigeonHole(lessonPages.size(), "");

    for (auto& page : lessonPages) {
        QString filename = page;
        filename.replace(precedingPath, "");
        QString indexStr = filename.split(".")[0];

        bool isNum;
        int index = indexStr.toInt(&isNum);

        if (isNum) {
            if (index >= 0 && index < pigeonHole.size()) {
                pigeonHole[index] = page;
            }
        }
    }

    lessonPages = std::move(pigeonHole);
}

Action Lesson::stringToAction(QString s) {
    if (s == "check") {
        return Action::Check;
    }

    if (s == "call") {
        return Action::Call;
    }

    if (s == "raise") {
        return Action::Raise;
    }

    if (s == "fold") {
        return Action::Fold;
    }

    return Action::None;
}

queue<Action> Lesson::getPlayerBotActions(int playerIndex) const {
    queue<Action> playerActions;

    for (const BotAction& action : currentBotActions) {
        if (action.player == playerIndex) {
            playerActions.push(action.action);
        }
    }

    return playerActions;
}

void Lesson::allowNext(bool allowed) {
    emit updateNext(allowed);
}
