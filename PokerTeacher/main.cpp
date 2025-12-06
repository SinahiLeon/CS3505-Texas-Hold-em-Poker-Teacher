#include "view.h"
#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game game;
    View view(game);
    // Initialize lesson
    // ...
    emit view.viewInitialized();
    view.show();
    return a.exec();
}
