#include "settingdialog.h"
#include "ui_settingdialog.h"

QString root_path;

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    ui->lineEdit_root_path->setText(root_path);

    connect(ui->pushBtn_open_root, SIGNAL(clicked(bool)), this, SLOT(pushBtn_open_root_click()));
    connect(ui->pushBtn_save, SIGNAL(clicked(bool)), this, SLOT(pushBtn_save_click()));
    connect(ui->pushBtn_cancel, SIGNAL(clicked(bool)), this, SLOT(pushBtn_cancel_click()));
}

void SettingDialog::pushBtn_open_root_click()
{
    root_path = get_directory_path();

    if(root_path == "")
        return;

    ui->lineEdit_root_path->setText(root_path);
}

void SettingDialog::pushBtn_save_click()
{    
    QFile file("./setting.ini");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug("file open fail");
        return;
    }

    QTextStream stream(&file);
    stream << root_path;
    file.close();

    accept();
}

void SettingDialog::pushBtn_cancel_click()
{
    reject();
}

QString SettingDialog::get_directory_path()
{
    return QFileDialog::getExistingDirectory(this, tr("Open Directory"),QDir::rootPath(),
                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}
