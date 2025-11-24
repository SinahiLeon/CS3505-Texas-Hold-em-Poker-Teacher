#include "view.h"
#include "./ui_view.h"

View::View(Game& game, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    connect(&game, &Game::chipsUpdated,
            this, &View::chipUpdate);
    connect(&game, &Game::communityCardsUpdated,
            this, &View::communityUpdate);
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
