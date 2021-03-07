#include "refdialog.h"
#include "ui_refdialog.h"

RefDialog::RefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RefDialog)
{
    ui->setupUi(this);

    connect(ui->pushBtn_ok, SIGNAL(clicked(bool)), this, SLOT(pushBtn_ok_click()));
    connect(ui->pushBtn_cancel, SIGNAL(clicked(bool)), this, SLOT(pushBtn_cancel_click()));
}

RefDialog::~RefDialog()
{
    delete ui;
}

void RefDialog::pushBtn_ok_click()
{
    display = ui->lineEdit_display->text();
    link = ui->lineEdit_link->text();

    accept();
}

void RefDialog::pushBtn_cancel_click()
{
    reject();
}
