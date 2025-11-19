#include "cardlibrary.h"
#include <QMap>

CardLibrary::CardLibrary() {}

const QMap<QString, Card> CardLibrary::cards = {
    {QString("Ace of Spades"), Card(Suit::Spade, CardValue(14), QImage("card_images/ace_of_spades.png"))}
};
