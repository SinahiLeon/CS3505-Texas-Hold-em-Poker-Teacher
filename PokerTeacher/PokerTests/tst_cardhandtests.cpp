#include <QtTest>

// add necessary includes here

class CardHandTests : public QObject
{
    Q_OBJECT
private slots:
    void returnsCorrrectCombinations();
};

void CardHandTests::returnsCorrrectCombinations() {

}

QTEST_APPLESS_MAIN(CardHandTests)
