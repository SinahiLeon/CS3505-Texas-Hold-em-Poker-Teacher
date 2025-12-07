#include "infobox.h"
#include "ui_infobox.h"

InfoBox::InfoBox(Lesson& _lesson, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::InfoBox)
    , lesson(_lesson)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    qDebug() << "ui setup";

    connect(ui->backButton, &QPushButton::clicked,
            this, &InfoBox::back);
    connect(ui->nextButton, &QPushButton::clicked,
            this, &InfoBox::next);
    connect(&lesson, &Lesson::updateNext,
            ui->nextButton, &QPushButton::setEnabled);
    connect(&lesson, &Lesson::updateDecision,
            this, &InfoBox::setDecision);
    connect(&lesson, &Lesson::displayFeedback,
            this, &InfoBox::displayFeedback);
    connect(this, &InfoBox::getDecision,
            &lesson, &Lesson::sendDecision);
    connect(&lesson, &Lesson::displayDecision,
            this, &InfoBox::displayDecision);

    qDebug() << "All connects";
    ui->lessonBrowser->setSource(lesson.getCurrentUrl());
    qDebug() << "Lesson src found";
}

InfoBox::~InfoBox() {
    delete ui;
}

void InfoBox::back() {
    bool hasPageHistory = lesson.back();

    if (hasPageHistory && !ui->nextButton->isEnabled()) {
        ui->nextButton->setEnabled(true);
    }

    if (lesson.atStart()) {
        ui->backButton->setEnabled(false);
    }

    ui->lessonBrowser->backward();
}

void InfoBox::next() {
    // This comes first so decisions only appear after a user has read through the game text
    if (decision.has_value()) {
        displayDecision(*decision);
        decision = nullopt;
        return;
    }

    bool hasNext = lesson.nextPage();

    if (hasNext && !ui->backButton->isEnabled()) {
        ui->backButton->setEnabled(true);
    }

    if (lesson.atEnd()) {
        ui->nextButton->setEnabled(false);
    }

    ui->lessonBrowser->setSource(lesson.getCurrentUrl());
}

void InfoBox::displayDecision(Decision& decision) {
    ui->nextButton->setEnabled(false);

    // Dialog content
    QMessageBox decisionBox;
    decisionBox.setText(decision.prompt);

    // Dialog buttons
    for (int i = 0; i < decision.choices.size(); i++) {
        QPushButton* choiceButton = new QPushButton(decision.choices[i]);

        connect(choiceButton, &QPushButton::clicked,
                &lesson, [this, i]() { lesson.makeChoice(i); });

        decisionBox.addButton(choiceButton, QMessageBox::AcceptRole);
    }

    decisionBox.exec();
}

void InfoBox::displayFeedback(QString feedback, bool correct) {
    QMessageBox feedbackBox;
    feedbackBox.setText(feedback);
    feedbackBox.setStandardButtons(QMessageBox::Ok);
    feedbackBox.exec();

    if (!correct) {
        emit getDecision();
    }
}
