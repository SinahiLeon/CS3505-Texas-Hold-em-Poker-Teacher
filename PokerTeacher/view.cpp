#include "view.h"
#include "./ui_view.h"
#include "cardlibrary.h"
#include <QString>
#include <QImageReader>
#include <qdir.h>
#include <infobox.h>
#include <QFile>
#include <QDialog>
#include <QPlainTextEdit>
#include <QVBoxLayout>

View::View(Game& game, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , game(game)
{
    ui->setupUi(this);

    connect(&game, &Game::chipsUpdated,
            this, &View::chipsUpdate);
    connect(&game, &Game::dealerUpdate,
            this, &View::dealerUpdate);
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
    connect(&game, &Game::updateLastAction,
            this, &View::updateLastAction);
    connect(&game, &Game::updateAvailableActions,
            this, &View::updateAvailableActions);
    connect(this, &View::viewInitialized,
            &game, &Game::onViewInitialized);
    connect(ui->checkButton, &QPushButton::clicked,
            this, &View::onCheckButtonClicked);
    connect(ui->betButton, &QPushButton::clicked,
            this, &View::onBetButtonClicked);
    connect(ui->callButton, &QPushButton::clicked,
            this, &View::onCallButtonClicked);
    connect(ui->foldButton, &QPushButton::clicked,
            this, &View::onFoldButtonClicked);
    connect(ui->infoButton, &QPushButton::clicked,
            this, &View::onInfoButtonClicked);
    connect(ui->actionLesson_1, &QAction::triggered,
            this, [this]() { this->onLessonActionClicked(1); });
    connect(ui->actionFreeplay, &QAction::triggered,
            this, &View::freeplayClicked);
    connect(ui->actionFreeplay, &QAction::triggered,
            &game, &Game::startNewGame);
}

View::~View()
{
    delete ui;
}

void View::updateAvailableActions() {
    bool canCheck = game.noBetsYetThisPhase && !(game.getPhase() == Phase::Showdown);
    ui->checkButton->setDisabled(!canCheck);
    bool canBet = game.players[0].canBet(game.getBetAmount()) && !(game.getPhase() == Phase::Showdown);
    ui->betButton->setDisabled(!canBet);
    ui->betAmount->setMaximum(game.players[0].getChips());
    bool canCall = game.players[0].canBet(game.getBetAmount()) && !(game.getPhase() == Phase::Showdown);
    ui->callButton->setDisabled(!canCall);
    qDebug() << "UI: Available actions: Check =" << canCheck << "Bet =" << canBet << "Call =" << canCall << "Fold = true";
}

void View::chipsUpdate(int playerIndex, int chips, int bet) {
    //update player at playerIndex's chip count to given newAmount
    switch(playerIndex) {
    case 0:  //0 is you
        ui->playerChipsLabel->setText(QString("Chips: $%1 - Bet: $%2").arg(chips).arg(bet));
        break;
    case 1: //1 is opponent 1
        ui->opp1ChipsLabel->setText(QString("Chips: $%1 - Bet: $%2").arg(chips).arg(bet));
        break;
    case 2: //2 is opponent 2
        ui->opp2ChipsLabel->setText(QString("Chips: $%1 - Bet: $%2").arg(chips).arg(bet));
        break;
    }
}

void View::potUpdate(int newAmount) {
    //update pot label to match pot amount
    ui->potLabel->setText("Pot: $" + QString::number(newAmount));
}

void View::currentBetUpdate(int currentBet) {
    ui->currentBetLabel->setText(QString("Current bet: $%1").arg(std::to_string(game.getBetAmount())));
    ui->betAmount->setMinimum(game.getBetAmount());
}

void View::onBetButtonClicked() {
    int amount = ui->betAmount->value();
    game.playerMakesBet(amount);
}

void View::onCallButtonClicked() {
    game.playerCalls();
}

void View::onCheckButtonClicked() {
    game.playerChecks();
}

void View::onFoldButtonClicked() {
    game.playerFolds();
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
                            ? ((game.getPhase() == Phase::Showdown)
                                   ? game.players[1].heldCards.at(0)->image
                                   : CardLibrary::cardBack)
                            : CardLibrary::noCard;
    QPixmap opp1card2 = (game.players[1].heldCards.size() > 1)
                            ? ((game.getPhase() == Phase::Showdown)
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
                            ? ((game.getPhase() == Phase::Showdown)
                                   ? game.players[2].heldCards.at(0)->image
                                   : CardLibrary::cardBack)
                            : CardLibrary::noCard;
    QPixmap opp2card2 = (game.players[2].heldCards.size() > 1)
                            ? ((game.getPhase() == Phase::Showdown)
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

void View::phaseUpdate(Phase currPhase) {
    //update phase label to match current phase
    switch(currPhase) {
    case Phase::Preflop:
        ui->phaseLabel->setText("Current Phase: Preflop");
        break;
    case Phase::Flop:
        ui->phaseLabel->setText("Current Phase: Flop");
        break;
    case Phase::Turn:
        ui->phaseLabel->setText("Current Phase: Turn");
        break;
    case Phase::River:
        ui->phaseLabel->setText("Current Phase: River");
        break;
    case Phase::Showdown:
        ui->phaseLabel->setText("Current Phase: Showdown");
        break;
    }
}

void View::updateLastAction(int playerIndex, QString action) {
    if (playerIndex == 0) {
        ui->playerLastAction->setText(QString("You ").append(action));
    }
    else {
        switch (playerIndex) {
            case (1) : { ui->opp1LastAction->setText(QString("Opponent 1 ").append(action)); break; }
            case (2) : { ui->opp2LastAction->setText(QString("Opponent 2 ").append(action)); break; }
        }
    }
}

void View::dealerUpdate(int playerIndex) {
    (playerIndex == 0) ? ui->playerName->setText(QString("You ♠"))
                       : ui->playerName->setText(QString("You"));
    (playerIndex == 1) ? ui->opp1Name->setText(QString("Opponent 1 ♠"))
                       : ui->opp1Name->setText(QString("Opponent 1"));
    (playerIndex == 2) ? ui->opp2Name->setText(QString("Opponent 2 ♠"))
                       : ui->opp2Name->setText(QString("Opponent 2"));
    switch (playerIndex) {
        case 0 : ui->dealerLabel->setText(QString("You are the Dealer this hand.")); break;
        case 1 : ui->dealerLabel->setText(QString("Opponent 1 is the Dealer this hand.")); break;
        case 2 : ui->dealerLabel->setText(QString("Opponent 2 is the Dealer this hand.")); break;
    }
}

void View::freeplayClicked() {
    qDebug() << "Freeplay button clicked.";
}

void View::onInfoButtonClicked()
{
    // File holds the main poker glossary text.
    QFile glossaryFile(":/texas_holdem_glossary.txt");
    QString glossaryText;

    if (glossaryFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        glossaryText = QString::fromUtf8(glossaryFile.readAll());
        glossaryFile.close();
    } else {
        glossaryText = "Failed to load Texas Holdem Glossary";
    }

    // File holds the ranked list of poker hands
    QFile hierarchyFile(":/hand_hierarchy.txt");
    QString hierarchyText;

    if (hierarchyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        hierarchyText = QString::fromUtf8(hierarchyFile.readAll());
        hierarchyFile.close();
    }

    else {
        hierarchyText = "Failed to load Hand Hierarchy";
    }

    // Create dialog window when user clicks glossary button
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Texas Hold'em GLossary & Hand Rankings");
    dialog->resize(600, 500);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(&*dialog);

    // Create 2 tabs so user can switch between the Glossary and Hand Hierarchy
    QTabWidget *tabs = new QTabWidget(&*dialog);

    // Tab 1: Glosssary
    QPlainTextEdit *glossaryBox = new QPlainTextEdit(&*dialog);
    glossaryBox->setReadOnly(true);
    glossaryBox->setPlainText(glossaryText);

    // Tab 2: Hand Hierarchy
    QPlainTextEdit *hierarchyBox = new QPlainTextEdit(&*dialog);
    hierarchyBox->setReadOnly(true);
    hierarchyBox->setPlainText(hierarchyText);

    // Add both tabs to tab widget
    tabs->addTab(glossaryBox, "Glossary");
    tabs->addTab(hierarchyBox, "Hand Rankings");

    layout->addWidget(tabs);
    dialog->setLayout(layout);

    // Show dialog box until user closes out
    dialog->show();
}

void View::onLessonActionClicked(int action) {
    qDebug() << "Opening info box.";
    Lesson* lesson = new Lesson(QString(":/Lessons/1-Hand_Types_and_Position"));
    InfoBox* infobox = new InfoBox(lesson, this);
    infobox->show();
}
