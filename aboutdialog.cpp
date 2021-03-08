#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size().width(), this->size().height());

    QString str = "Developer: sakusi4\n";
    str += "Homepage: https://sakusi4.github.io\n";
    str += "Source code: https://github.com/sakusi4/MDWriter";

    ui->textEdit_about->setPlainText(str);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
