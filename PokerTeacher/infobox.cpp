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
    bool hasNext = lesson.nextPage();

    if (hasNext && !ui->backButton->isEnabled()) {
        ui->backButton->setEnabled(true);
    }

    if (lesson.atEnd()) {
        ui->nextButton->setEnabled(false);
    }

    ui->lessonBrowser->setSource(lesson.getCurrentUrl());
}
