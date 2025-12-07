#include "view.h"
#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game game;
    View view(game);
    view.show();
    emit view.viewInitialized();
    return a.exec();
}
