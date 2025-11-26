#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(Game& game, QWidget *parent = nullptr);
    ~View();

signals:
    void viewInitialized();

public slots:
    void chipUpdate(int playerIndex, int newAmount);
    void potUpdate(int newAmount);
    void currentBetUpdate(int currentBet);
    void communityUpdate();
    void handsUpdate();
    void phaseUpdate(Phase currPhase);
    void updateLastAction(int playerIndex, QString action);
    void updateAvailableActions();
    void onBetButtonClicked();
    void onCallButtonClicked();
    void onCheckButtonClicked();
    void onFoldButtonClicked();
private:
    Ui::View *ui;
    Game& game;
};
#endif // VIEW_H
