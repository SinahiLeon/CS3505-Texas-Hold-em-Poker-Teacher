#include <QTest>
#include "tst_cardhandtests.cpp"
#include "tst_lessontests.cpp"

int main(int argc, char *argv[])
{
    int status = 0;
    CardHandTests cht;
    LessonTests lt;

    status |= QTest::qExec(&cht, argc, argv);
    status |= QTest::qExec(&lt, argc, argv);

    return status;
}
