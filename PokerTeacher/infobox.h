#ifndef INFOBOX_H
#define INFOBOX_H

#include <QDialog>
#include <QMessageBox>
#include "lesson.h"

using std::nullopt;

namespace Ui {
class InfoBox;
}

class InfoBox : public QDialog
{
    Q_OBJECT

public:
    explicit InfoBox(Lesson& _lesson, QWidget* parent = nullptr);
    ~InfoBox();

public slots:
    void setDecision(Decision d) { decision = optional<Decision>(d); };

signals:
    void getDecision();

private:
    Ui::InfoBox *ui;
    Lesson& lesson;
    optional<Decision> decision = nullopt;

    void displayDecision(Decision& decision);
    void displayFeedback(QString feedback, bool correct);

private slots:
    void back();
    void next();
};

#endif // INFOBOX_H
