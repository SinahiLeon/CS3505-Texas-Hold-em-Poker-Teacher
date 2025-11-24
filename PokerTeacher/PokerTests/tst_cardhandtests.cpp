#include <QTest>
#include <memory>
#include "cardhand.h"
#include "cardlibrary.h"
using std::make_shared;

class CardHandTests : public QObject
{
Q_OBJECT

private:
    vector<shared_ptr<Card>> genHand(vector<QString> cards);
private slots:
    void constructs();
    void onePair();
    void twoPair();
    void threeKind();
    void straight();
    void lowAceStraight();
    void duplicateStraight();
    void flush();
    void fullHouse();
    void fourKind();
    void straightFlush();
    void royalFlush();
};

void CardHandTests::constructs() {
    vector<shared_ptr<Card>> startHand = genHand({"Ace of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    QCOMPARE(hand.getHandType(), HandType::HighCard);
    QCOMPARE(hand.getBestCard(), startHand[0]);
}

void CardHandTests::onePair() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"7 of Hearts", "3 of Hearts", "Queen of Clubs", "9 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::OnePair);
    QCOMPARE(hand.getBestCard()->value, CardValue::Seven);
}

void CardHandTests::twoPair() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"7 of Hearts", "3 of Hearts", "Queen of Clubs", "9 of Diamonds", "3 of Clubs"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::TwoPair);
    QCOMPARE(hand.getBestCard()->value, CardValue::Seven);
}

void CardHandTests::threeKind() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"7 of Clubs", "7 of Hearts", "Queen of Clubs", "9 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::ThreeKind);
    QCOMPARE(hand.getBestCard()->value, CardValue::Seven);
}

void CardHandTests::straight() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "8 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"2 of Spades", "10 of Hearts", "Queen of Clubs", "9 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::Straight);
    QCOMPARE(hand.getBestCard()->value, CardValue::Jack);
}

void CardHandTests::lowAceStraight() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "Ace of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"2 of Spades", "3 of Hearts", "5 of Clubs", "4 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::Straight);
    QCOMPARE(hand.getBestCard()->value, CardValue::Five);
}

void CardHandTests::duplicateStraight() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "8 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"7 of Hearts", "10 of Hearts", "Queen of Clubs", "9 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::Straight);
    QCOMPARE(hand.getBestCard()->value, CardValue::Jack);
}

void CardHandTests::flush() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "8 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"7 of Spades", "10 of Spades", "Queen of Spades", "9 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::Flush);
    QCOMPARE(hand.getBestCard()->value, CardValue::Queen);
}

void CardHandTests::fullHouse() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"7 of Hearts", "7 of Clubs", "2 of Clubs", "9 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::FullHouse);
    QCOMPARE(hand.getBestCard()->value, CardValue::Seven);
}

void CardHandTests::fourKind() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"7 of Diamonds", "7 of Hearts", "7 of Clubs", "9 of Diamonds", "Jack of Hearts"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::FourKind);
    QCOMPARE(hand.getBestCard()->value, CardValue::Seven);
}

void CardHandTests::straightFlush() {
    vector<shared_ptr<Card>> startHand = genHand({"7 of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"8 of Spades", "7 of Hearts", "10 of Spades", "9 of Spades", "Jack of Spades"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::StraightFlush);
    QCOMPARE(hand.getBestCard()->value, CardValue::Jack);
}

void CardHandTests::royalFlush() {
    vector<shared_ptr<Card>> startHand = genHand({"10 of Spades", "2 of Spades"});
    CardHand hand = CardHand(startHand);

    vector<shared_ptr<Card>> river = genHand({"Jack of Spades", "7 of Hearts", "King of Spades", "Queen of Spades", "Ace of Spades"});
    hand.calculateBestHand(river);

    QCOMPARE(hand.getHandType(), HandType::RoyalFlush);
    QCOMPARE(hand.getBestCard()->value, CardValue::Ace);
}

vector<shared_ptr<Card>> CardHandTests::genHand(vector<QString> cards) {
    vector<shared_ptr<Card>> hand;

    for (int x = 0; x < cards.size(); x++) {
        shared_ptr<Card> card = make_shared<Card>(CardLibrary::cards.at(x));
        hand.push_back(card);
    }

    return hand;
}

QTEST_APPLESS_MAIN(CardHandTests)

#include "tst_cardhandtests.moc"
