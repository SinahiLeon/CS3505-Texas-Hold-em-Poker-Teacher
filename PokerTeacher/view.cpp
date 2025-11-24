#include "view.h"
#include "./ui_view.h"
#include "cardlibrary.h"
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
void View::communityUpdate() {
    //update displayed community cards to match backend
    //TODO DISPLAY CARD IMAGES
    qDebug() << "Drawing community cards.";
    qDebug() << "NOT IMPLEMENTED";

}
void View::handsUpdate() {
    qDebug() << "Drawing hand cards.";
    // ui->playerCard1->setPixmap(QPixmap::fromImage(CardLibrary::cardBack));
    // ui->playerCard2->setPixmap(QPixmap::fromImage(CardLibrary::cardBack));


    // ui->playerCard2->setPixmap(QPixmap::fromImage(CardLibrary::cardBack));
    ui->playerCard1->setPixmap((game.players[0].heldCards.size() > 0) ?
                                                    game.players[0].heldCards.at(0)->image :
                                                    CardLibrary::noCard);
    ui->playerCard2->setPixmap((game.players[0].heldCards.size() > 1) ?
                                                    game.players[0].heldCards.at(1)->image :
                                                    CardLibrary::noCard);
    ui->playerCard1->setPixmap(QPixmap(":/Resources/card_images/ace_of_spades.png"));
    ui->playerCard1->setScaledContents(true);
    ui->playerCard2->setScaledContents(true);
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
