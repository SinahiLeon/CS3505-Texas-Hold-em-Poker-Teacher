#ifndef GLOSSARYDIALOG_H
#define GLOSSARYDIALOG_H

#include <QDialog>
#include "definition.h"

namespace Ui {
    class GlossaryDialog;
}

class GlossaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlossaryDialog(Definition def, QWidget *parent = nullptr);
    ~GlossaryDialog();

private:
    Ui::GlossaryDialog *ui;
};

#endif // GLOSSARYDIALOG_H
