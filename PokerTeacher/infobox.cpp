#include "infobox.h"
#include "ui_infobox.h"

InfoBox::InfoBox(Lesson& lesson, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InfoBox)
{
    ui->setupUi(this);

    connect(this, &QDialog::finished,
            this, &QObject::deleteLater);

    setupLesson(lesson);
}

InfoBox::~InfoBox()
{
    delete ui;
}

void InfoBox::setupLesson(Lesson& lesson) {
    ui->lessonBrowser->setSource(lesson.getCurrentUrl());
}
