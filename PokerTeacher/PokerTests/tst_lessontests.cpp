#include <QTest>
#include "lesson.h"

using std::nullopt;

class LessonTests : public QObject
{
    Q_OBJECT

private slots:
    void constructs();
    void getsAllPages();
    void getsNextPage();
    void getsNextLesson();
};

void LessonTests::constructs() {
    Lesson newLesson(QString(":/Lessons/1-TestLesson"));
    QCOMPARE(newLesson.getLessonNum(), 1);
    QCOMPARE(newLesson.getCurrentPage(), QString(":/Lessons/1-TestLesson/0.html"));
}

void LessonTests::getsAllPages() {
    vector<QString> pages { QString(":/Lessons/1-TestLesson/0.html"), QString(":/Lessons/1-TestLesson/1.html") };
    Lesson newLesson(QString(":/Lessons/1-TestLesson"));
    vector<QString> lessonPages = newLesson.getLessonPages();
    QCOMPARE(pages, lessonPages);
}

void LessonTests::getsNextPage() {
    Lesson newLesson(QString(":/Lessons/1-TestLesson"));
    newLesson.nextPage();
    QCOMPARE(newLesson.getCurrentPage(), QString(":/Lessons/1-TestLesson/1.html"));
}

void LessonTests::getsNextLesson() {
    Lesson newLesson(QString(":/Lessons/1-TestLesson"));
    optional<Lesson> nextLesson = newLesson.getNextLesson();
    QVERIFY(nextLesson.has_value());
    QCOMPARE(nextLesson->getCurrentPage(), QString(":/Lessons/2-TestLesson/1.html"));
}

// Move this line between test classes to run different classes.
QTEST_APPLESS_MAIN(LessonTests)

#include "tst_lessontests.moc"
