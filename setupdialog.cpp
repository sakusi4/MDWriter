#include "setupdialog.h"
#include "ui_setupdialog.h"

#include <QDateTime>
#include <QMessageBox>

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);    
    this->setFixedSize(this->size().width(), this->size().height());

    //QString time_format = "yyyy-MM-dd  HH:mm:ss";
    QString time_format = "yyyy-MM-dd";
    QDateTime cur_time = QDateTime::currentDateTime();
    QString md_date = cur_time.toString(time_format);

    ui->lineEdit_file_name->setText(md_date + "-");
    ui->lineEdit_date->setText(md_date);

    connect(ui->pushBtn_ok, SIGNAL(clicked(bool)), this, SLOT(pushBtn_ok_click()));
    connect(ui->pushBtn_cancel, SIGNAL(clicked(bool)), this, SLOT(pushBtn_cancel_click()));
}


void SetupDialog::pushBtn_ok_click()
{
    layout = ui->lineEdit_layout->text();
    file_name = ui->lineEdit_file_name->text();
    title = ui->lineEdit_title->text();
    category = ui->lineEdit_category->text();
    date = ui->lineEdit_date->text();

    if(file_name.length() <= 11 || layout == "" || title == "" || category == "" || date == ""){
        QMessageBox::information(this, "info", "please check datas");
        return;
    }

    accept();
}

void SetupDialog::pushBtn_cancel_click()
{
    reject();
}

SetupDialog::~SetupDialog()
{
    delete ui;
}
