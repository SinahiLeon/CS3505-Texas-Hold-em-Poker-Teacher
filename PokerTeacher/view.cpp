#include "view.h"
#include "./ui_view.h"
#include "cardlibrary.h"
#include <QString>
#include <QImageReader>
#include <qdir.h>

View::View(Game& game, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , game(game)
{
    ui->setupUi(this);

    connect(&game, &Game::chipsUpdated,
            this, &View::chipUpdate);
    connect(&game, &Game::communityCardsUpdated,
            this, &View::communityUpdate);
    connect(&game, &Game::handCardsUpdated,
            this, &View::handsUpdate);
    connect(&game, &Game::potUpdated,
            this, &View::potUpdate);
    connect(&game, &Game::currentBetUpdated,
            this, &View::currentBetUpdate);
    connect(&game, &Game::phaseUpdated,
            this, &View::phaseUpdate);
    connect(this, &View::viewInitialized,
            &game, &Game::onViewInitialized);
}

View::~View()
{
    delete ui;
}

void View::chipUpdate(int playerIndex, int newAmount) {
    //update player at playerIndex's chip count to given newAmount
    switch(playerIndex) {
    case 0:  //0 is you
        ui->playerChipsLabel->setText("Chips: $" + QString::number(newAmount));
        break;
    case 1: //1 is opponent 1
        ui->opp1ChipsLabel->setText("Chips: $" + QString::number(newAmount));
        break;
    case 2: //2 is opponent 2
        ui->opp2ChipsLabel->setText("Chips: $" + QString::number(newAmount));
        break;
    }
}

void View::potUpdate(int newAmount) {
    //update pot label to match pot amount
    ui->potLabel->setText("Pot: $" + QString::number(newAmount));
}

void View::currentBetUpdate(int currentBet) {
    ui->currentBetLabel->setText(QString("Current bet: $%1").arg(std::to_string(game.getCurrentBet())));
    ui->betAmount->setMinimum(game.getCurrentBet());
}

void View::communityUpdate() {
    //update displayed community cards to match backend
    //TODO DISPLAY CARD IMAGES
    qDebug() << "UI: Updating community cards.";
    QSize commCardSize(80,110);
    QPixmap comm1 = (game.communityCards.size() > 0)
                        ? game.communityCards.at(0)->image
                        : CardLibrary::noCard;
    QPixmap comm2 = (game.communityCards.size() > 1)
                        ? game.communityCards.at(1)->image
                        : CardLibrary::noCard;
    QPixmap comm3 = (game.communityCards.size() > 2)
                        ? game.communityCards.at(2)->image
                        : CardLibrary::noCard;
    QPixmap comm4 = (game.communityCards.size() > 3)
                        ? game.communityCards.at(3)->image
                        : CardLibrary::noCard;
    QPixmap comm5 = (game.communityCards.size() > 4)
                        ? game.communityCards.at(4)->image
                        : CardLibrary::noCard;
    ui->commCard1->setPixmap(comm1);
    ui->commCard2->setPixmap(comm2);
    ui->commCard3->setPixmap(comm3);
    ui->commCard4->setPixmap(comm4);
    ui->commCard5->setPixmap(comm5);
    ui->commCard1->setScaledContents(true);
    ui->commCard2->setScaledContents(true);
    ui->commCard3->setScaledContents(true);
    ui->commCard4->setScaledContents(true);
    ui->commCard5->setScaledContents(true);
    ui->commCard1->setFixedSize(commCardSize);
    ui->commCard2->setFixedSize(commCardSize);
    ui->commCard3->setFixedSize(commCardSize);
    ui->commCard4->setFixedSize(commCardSize);
    ui->commCard5->setFixedSize(commCardSize);
}

void View::handsUpdate() {
    qDebug() << "UI: Updating hand cards.";
    QSize playerCardSize(100,140);
    QSize opponentCardSize(80,110);
    // Player
    QPixmap card1 = (game.players[0].heldCards.size() > 0)
                        ? game.players[0].heldCards.at(0)->image
                        : CardLibrary::noCard;
    QPixmap card2 = (game.players[0].heldCards.size() > 1)
                        ? game.players[0].heldCards.at(1)->image
                        : CardLibrary::noCard;
    ui->playerCard1->setPixmap(card1);
    ui->playerCard2->setPixmap(card2);
    ui->playerCard1->setScaledContents(true);
    ui->playerCard2->setScaledContents(true);
    ui->playerCard1->setFixedSize(playerCardSize);
    ui->playerCard2->setFixedSize(playerCardSize);
    // Opponent 1
    QPixmap opp1card1 = (game.players[1].heldCards.size() > 0)
                            ? ((game.getPhase() == Game::Phase::Showdown)
                                   ? game.players[1].heldCards.at(0)->image
                                   : CardLibrary::cardBack)
                            : CardLibrary::noCard;
    QPixmap opp1card2 = (game.players[1].heldCards.size() > 1)
                            ? ((game.getPhase() == Game::Phase::Showdown)
                                   ? game.players[1].heldCards.at(1)->image
                                   : CardLibrary::cardBack)
                            : CardLibrary::noCard;
    ui->opp1Card1->setPixmap(opp1card1);
    ui->opp1Card2->setPixmap(opp1card2);
    ui->opp1Card1->setScaledContents(true);
    ui->opp1Card2->setScaledContents(true);
    ui->opp1Card1->setFixedSize(opponentCardSize);
    ui->opp1Card2->setFixedSize(opponentCardSize);
    // Opponent 2
    QPixmap opp2card1 = (game.players[2].heldCards.size() > 0)
                            ? ((game.getPhase() == Game::Phase::Showdown)
                                   ? game.players[2].heldCards.at(0)->image
                                   : CardLibrary::cardBack)
                            : CardLibrary::noCard;
    QPixmap opp2card2 = (game.players[2].heldCards.size() > 1)
                            ? ((game.getPhase() == Game::Phase::Showdown)
                                   ? game.players[2].heldCards.at(1)->image
                                   : CardLibrary::cardBack)
                            : CardLibrary::noCard;
    ui->opp2card1->setPixmap(opp2card1);
    ui->opp2card2->setPixmap(opp2card2);
    ui->opp2card1->setScaledContents(true);
    ui->opp2card2->setScaledContents(true);
    ui->opp2card1->setFixedSize(opponentCardSize);
    ui->opp2card2->setFixedSize(opponentCardSize);
}

void View::phaseUpdate(Game::Phase currPhase) {
    //update phase label to match current phase
    switch(currPhase) {
    case Game::Phase::Preflop:
        ui->phaseLabel->setText("Current Phase: Preflop");
        break;
    case Game::Phase::Flop:
        ui->phaseLabel->setText("Current Phase: Flop");
        break;
    case Game::Phase::Turn:
        ui->phaseLabel->setText("Current Phase: Turn");
        break;
    case Game::Phase::River:
        ui->phaseLabel->setText("Current Phase: River");
        break;
    case Game::Phase::Showdown:
        ui->phaseLabel->setText("Current Phase: Showdown");
        break;
    }
}
