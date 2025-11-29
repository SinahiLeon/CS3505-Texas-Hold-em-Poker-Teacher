#include "glossarydialog.h"
#include "ui_glossarydialog.h"

GlossaryDialog::GlossaryDialog(Definition def, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GlossaryDialog)
{
    ui->setupUi(this);

    ui->nameLabel->setText(def.name);
    ui->definitionText->setPlainText(def.definiton);
}

GlossaryDialog::~GlossaryDialog()
{
    delete ui;
}
