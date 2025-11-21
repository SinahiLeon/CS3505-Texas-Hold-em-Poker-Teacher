#include "view.h"
#include "./ui_view.h"

View::View(Game& game, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    connect(game, &Game::chipsUpdated,
            this, &View::chipUpdate);
}

View::~View()
{
    delete ui;
}

void View::chipUpdate(int playerIndex, int newAmount) {

}
void View::potUpdate(int newAmount) {

}
void View::communityUpdate() {

}
void View::newPhase(Game::Phases currPhase) {

}
