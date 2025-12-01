#include "lesson.h"
#include <QRegularExpression>
using std::bad_cast;
using std::out_of_range;
using std::nullopt;

Lesson::Lesson(QString folderPath, QObject *parent): folder(QDir(folderPath)) {
    readFolderName();
    findLessonFiles();
}

Lesson::Lesson(const Lesson& other)
    : folder(other.folder)
    , name(other.name)
    , lessonPages(other.lessonPages)
    , cpus(other.cpus)
    , lessonNum(other.lessonNum)
{
    currentPage = lessonPages[0];
    pageIndex = 0;
}

bool Lesson::nextPage() {
    if (pageIndex >= lessonPages.size() - 1) {
        return false;
    }

    pageIndex++;
    currentPage = lessonPages[pageIndex];
    return true;
}

optional<Lesson> Lesson::getNextLesson() {
    int nextIndex = lessonNum + 1;
    QDir parent = folder;
    // This cannot return false in this context as this method is always run in a subfolder of Lessons.
    parent.cdUp();

    optional<QString> nextPath = findNextLesson(parent, nextIndex);

    if (nextPath.has_value()) {
        return optional<Lesson>(Lesson(*nextPath));
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
        // bad_cast exceptions do not include messages.
        throw bad_cast();
    }

    lessonNum = num;
    name = nameParts[1];
}

void Lesson::findLessonFiles() {
    // Gets files and sorts them by name.
    QStringList allFiles = folder.entryList(QDir::Files, QDir::Name);

    // Using const here detaches allFiles.
    for (auto& file : allFiles) {
        if (isHtmlFile(file)) {
            lessonPages.push_back(file);
        }
    }
}

bool Lesson::isHtmlFile(QString filename) {
    QStringList fileParts = filename.split(".");

    if (fileParts.size() > 1 && fileParts[fileParts.size() - 1] == "html") {
        return true;
    }

    return false;
}

optional<QString> Lesson::findNextLesson(QDir parentDir, int index) {
    QRegularExpression previousPath(".*/");
    QStringList allFolders = parentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (auto& folderPath : allFolders) {
        QString relativePath = folderPath.remove(previousPath);
        QStringList nameParts = folderPath.split("-");

        if (nameParts.size() > 0) {
            bool isNum;
            int num = nameParts[0].toInt(&isNum);

            if (isNum && num == index) {
                return folderPath;
            }
        }
    }

    return nullopt;
}
