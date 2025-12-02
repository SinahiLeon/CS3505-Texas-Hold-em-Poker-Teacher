#include "infobox.h"
#include "ui_infobox.h"

InfoBox::InfoBox(Lesson *lesson, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InfoBox)
    , lesson(lesson)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(this, &QDialog::finished,
            this, &QObject::deleteLater);
    connect(ui->backButton, &QPushButton::clicked,
            this, &InfoBox::back);
    connect(ui->nextButton, &QPushButton::clicked,
            this, &InfoBox::next);

    ui->lessonBrowser->setSource(lesson->getCurrentUrl());
}

InfoBox::~InfoBox()
{
    delete ui;
    delete lesson;
}

void InfoBox::back() {
    bool hasPageHistory = lesson->back();

    if (hasPageHistory && !ui->nextButton->isEnabled()) {
        ui->nextButton->setEnabled(true);
    }

    if (lesson->atStart()) {
        ui->backButton->setEnabled(false);
    }

    ui->lessonBrowser->backward();
}

void InfoBox::next() {
    bool hasNext = lesson->nextPage();

    if (hasNext && !ui->backButton->isEnabled()) {
        ui->backButton->setEnabled(true);
    }

    if (lesson->atEnd()) {
        ui->nextButton->setEnabled(false);
    }

    ui->lessonBrowser->setSource(lesson->getCurrentUrl());
}
