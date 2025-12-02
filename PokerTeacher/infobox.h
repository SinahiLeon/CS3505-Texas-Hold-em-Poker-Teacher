#ifndef INFOBOX_H
#define INFOBOX_H

#include <QDialog>
#include <qmainwindow.h>
#include "lesson.h"

namespace Ui {
class InfoBox;
}

class InfoBox : public QDialog
{
    Q_OBJECT

public:
    explicit InfoBox(Lesson *lesson, QWidget *parent = nullptr);
    ~InfoBox();

private:
    Ui::InfoBox *ui;
    Lesson *lesson;

private slots:
    void back();
    void next();
};

#endif // INFOBOX_H
